if {1 || [adapter_name] == "" } {
 source [find olimex-arm-usb-ocd-h.cfg]
 adapter_khz 8000
 #ftdi_tdo_sample_edge falling
 reset_config trst_only ;#trst_open_drain
 transport select jtag
}

if { [jtag names] == "" } {
  jtag newtap bj sata -irlen 8  -expected-id 0xa21c64cd -irmask 3 -disable
  jtag newtap bj ddr  -irlen 8  -expected-id 0x00002863 -irmask 3 -disable
  jtag newtap bj apb  -irlen 5  -expected-id 0x00001863 -irmask 3
  jtag newtap bj pci  -irlen 8  -expected-id 0x061c74cd -irmask 3 -disable
  jtag newtap bj xgbe -irlen 8  -expected-id 0x061c74cd -irmask 3 -disable
  jtag newtap bj top  -irlen 16 -expected-id 0x00000863 -irmask 3
}

jtag_ntrst_delay 10
jtag_ntrst_assert_width 100

# all disabled taps enables together
proc bjtap {} {}
jtag configure bj.sata -event tap-enable  bjtap
jtag configure bj.sata -event tap-disable bjtap
jtag configure bj.ddr  -event tap-enable bjtap
jtag configure bj.ddr  -event tap-disable bjtap
jtag configure bj.pci  -event tap-enable bjtap
jtag configure bj.pci  -event tap-disable bjtap
jtag configure bj.xgbe -event tap-enable bjtap
jtag configure bj.xgbe -event tap-disable bjtap


set bc_base     0x1f040000
set spib_base   0x1f040100
set pmu_base    0x1f04d000
set gpio_base   0x1f044000
set spi0_base   0x1f04e000
set spi1_base   0x1f04f000

set pmu_core        [expr ${pmu_base} + 0x028]
set pmu_ddrc        [expr ${pmu_base} + 0x02c]
set pmu_core_debug  [expr ${pmu_base} + 0x118]

# not implemented APB_RESTART 0x04 APB_HALT 0x08
set bj_safe true

array set japc {
 SCAN_N 2 APB_IDCODE 0x1E
 SCAN_ITR 4 SCAN_DTR 5
 EXTEST 0 INTEST 0xC
 IDCODE 0x1 BYPASS 0XFFFF CRSEL 0x31
 TT_SHORT 0xA001 TT_NORMAL 0xA000 TT_DFT_RESET 0xC000 TT_BIST_RUN 0xB000 TT_BIST_RESULT 0xC100 TT_BIST_RESULT_LENGTH 53
}

proc jtt_long {} {
  global japc
  irscan bj.top $japc(TT_NORMAL)
  runtest 2
  foreach tap [jtag names] {
    if { ![jtag tapisenabled $tap] } { jtag tapenable $tap }
  }
}

proc bj_soft_reset {} { 
  japb_set_reg $::pmu_core_debug 0x1 
}
# turn coreclk off
proc bj_coreclk {on} {
  set ::bj_safe true
  set v [japb_get_reg ${::pmu_base}]
  japb_set_reg ${::pmu_base} [expr $on ? $v|0x1 : $v & ~0x1]
  set ::bj_safe $on
}
proc bj_spib_unlock {} {
  japb_set_reg ${::bc_base} 0x100
}
proc bj_spib_lock {} {
  japb_set_reg ${::bc_base} [expr [japb_get_reg ${::bc_base}] & ~0x100]
}



#jtag configure bj.top -event setup jtt_long

proc japb_get_reg {addr {len 1}} {
  global japc
  irscan bj.apb $japc(SCAN_N)
  drscan bj.apb 5 $japc(SCAN_ITR)
  set ret {}
  while { [incr len -1] >= 0 } {
    irscan bj.apb $japc(EXTEST)
    drscan bj.apb 32 $addr 2 0
    for {set retry 50} {[incr retry -1]} {} {
      irscan bj.apb $japc(INTEST)
      set r [drscan bj.apb 32 0 2 0]
      #puts [format "r %08X: %s" $addr $r]
      if { [lindex $r 1] == "00" } { lappend ret "0x[lindex $r 0]"; break }
     }
     if { $retry == 0 } {
        error [format "Timeout reading register 0x%04X: $ret" $addr]
     }
    incr addr 4
  }
  return $ret
}

proc japb_set_reg {addr value} {
  global japc
  irscan bj.apb $japc(SCAN_N)
  drscan bj.apb 5 $japc(SCAN_DTR)
  irscan bj.apb $japc(EXTEST)
  drscan bj.apb 32 $value

  irscan bj.apb $japc(SCAN_N)
  drscan bj.apb 5 $japc(SCAN_ITR)
  irscan bj.apb $japc(EXTEST)
  drscan bj.apb 32 $addr 2 1
  if {![expr $::bj_safe ? 1:0]} return   ;# works stable if cores are stopped
  irscan bj.apb $japc(INTEST)
  # up to 10 retries sometimes required! (CPU in WFI?)
  for {set retry 50} {[incr retry -1]} {} {
    set r [drscan bj.apb 32 0 2 0]
    if { [lindex $r 1] == "00" } { return; } else { puts -nonewline . }
  }
  error [format "Timeout writing register 0x%04X" $addr]
}

proc bj_dft_reset {{val 0x1F}} {
  global japc
  irscan bj.top [expr $japc(TT_DFT_RESET) | ($val & 0x1f)]
  runtest 4
  return
}

proc bj_bist_start {val} {
  global japc
  irscan bj.top [expr $japc(TT_BIST_RUN) | ($val & 0xfff)]
  runtest 2
  return
}

proc bj_bist_result {} {
  global japc
  irscan bj.top $japc(TT_BIST_RESULT)
  # 4 done 1 TR_fail 16 P0_fail 16 P1_fail 16 L2_fail
  drscan bj.top 4 0 1 0 16 0 16 0 16 0
}

proc bj_bist_run {{testsel 0xF00}} {
  runtest 64
  japb_get_reg ${::pmu_core}          ;# ensure working
  japb_set_reg ${::pmu_core} 0x81     ;# core1 oper
  japb_set_reg ${::pmu_core_debug} 1  ;# reset, self-clear
  runtest 64
  bj_dft_reset 0x1F
  runtest 64
  bj_bist_start $testsel
  set retry 2000
  while {[incr retry -1]} {
    runtest 256
    set rx [bj_bist_result]
    set done "0x[lindex $rx 0]"
    if { ($done & 0xF) == ($testsel >> 8) } {
      bj_dft_reset 0x00
      return $rx
    }
  }
  error [format "Bist test timeout: command %X result %s" $testsel $rx]
}

proc ddr_init {} {
  japb_set_reg $::pmu_ddrc 0x6 
  japb_set_reg $::pmu_ddrc 0x4  ;# keep in INIT for CFG registers access
}

proc ddr_get_reg {addr} {
  global japc
  if { ![jtag tapisenabled bj.ddr] } { error "tap is not enabled" }
  irscan bj.ddr $japc(CRSEL)
  #drscan bj.ddr 43 [expr ($addr << 2) | 0x1]
  drscan bj.ddr 2 01 10 $addr 32 0
  runtest 15 ;# 8+DWC_CFG_OUT_PIPE
  return "0x[lindex [drscan bj.ddr 2 01 10 $addr 32 0] 2]"
}

proc ddr_set_reg {addr value} {
  global japc
  if { ![jtag tapisenabled bj.ddr] } { error "tap is not enabled" }
  irscan bj.ddr $japc(CRSEL)
  drscan bj.ddr 2 01 10 $addr 32 $value
  runtest 5
}

proc bj_eespi_init {spi {div 4}} {
  japb_set_reg [expr $spi + 0x08] 0x0  ;# SSI_EN=0: config
# perhaps requires CPOL=CPHA=1 for Atmel EEPROM devices?
  japb_set_reg [expr $spi + 0x10] 0x0  ;# Release CS#
  japb_set_reg [expr $spi + 0x14] $div ;# BAUD divisor
  return 8;
  # determine TX FIFO depth
  for {set n 8} {$n} {incr n} {
    japb_set_reg [expr $spi + 0x18] $n
    if { [japb_get_reg [expr $spi + 0x18]] != $n } { break };
  }
  return $n
}

proc bj_spi_cmd {spi ss out inlen} {
  set ret {}
#  japb_set_reg [expr $spi + 0x08] 0x0  ;# SSI_EN=0: config
  japb_set_reg [expr $spi + 0x10] 0x0  ;# Release CS#
  japb_set_reg [expr $spi + 0x00] [expr $inlen ? 0x307 : 0x107]
  japb_set_reg [expr $spi + 0x04] [expr $inlen-1]
  japb_set_reg [expr $spi + 0x08] 0x1  ;# SSI_EN=1
  set txl  [llength $out]
  set fifo [expr $spi == $::spib_base ? 8 : 64]
  if { $txl  > $fifo } { set txl $fifo }
  set started 0
  foreach d $out {
    japb_set_reg [expr $spi + 0x60] $d
    if { [incr txl -1] == 0 } {
      if { !$started } {
        japb_set_reg [expr $spi + 0x10] [expr 1 << $ss]   ;# Slave Enable
        set started 1
      }
      while { [set txl [expr $fifo - [japb_get_reg [expr $spi + 0x20]]]] == 0 } {}
    }
  }

  set ret {}
  # until TFE=1 & BUSY=0
  while { [set a [japb_get_reg [expr $spi + 0x24]]] != 0 || [[japb_get_reg [expr $spi + 0x28]] ^ 4] & 5 } {
    if { $a == 0 } { continue }
    foreach i [japb_get_reg [expr $spi + 0x60] $a] { lappend ret [format "0x%02X" $i] }
  }
  return $ret
}

# assume already init
proc bj_eespi_read_id {spi {ss 0} {len 3}}        { return [bj_spi_cmd $spi $ss 0x9f $len] }
proc bj_eespi_read_status   {spi {ss 0}}          { return [bj_spi_cmd $spi $ss 0x05 1] }
proc bj_eespi_write_status  {spi data {ss 0}}     { return [bj_spi_cmd $spi $ss [concat 0x01 $data] 0] }
proc bj_eespi_write_enable  {spi {ss 0}}          { return [bj_spi_cmd $spi $ss 0x06 0] }
proc bj_eespi_write_disable {spi {ss 0}}          { return [bj_spi_cmd $spi $ss 0x04 0] }

proc bj_spib_protect {{on 1}} {
  bj_coreclk on
  bj_spib_unlock
  bj_eespi_init $::spib_base 4000
  #echo [bj_eespi_read_id $::spib_base 0 20]
  bj_eespi_write_enable $::spib_base
  bj_eespi_write_status $::spib_base [expr $on ? 0x7e : 0x0]
  echo [bj_eespi_read_status $::spib_base]
}
proc bbb {} { bj_spib_protect 1 }
proc bba {} { bj_spib_protect 0 }
# vim: ft=tcl:ts=2:et:
