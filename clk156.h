#ifndef __CLK156_H__
#define __CLK156_H__


#ifdef __cplusplus
extern "C" {
#endif

bool clk156_set_override(unsigned address, unsigned value);
bool clk156_init(i2caddr_t addr);
bool clk156_verify(i2caddr_t addr, unsigned* failreg);
bool clk156_program_nvm(i2caddr_t addr);
bool clk156_read_nvm_bank(i2caddr_t addr, int bank);
uint32_t clk156_free_banks(i2caddr_t addr);
bool clk156_check(BaseSequentialStream *chp, i2caddr_t addr);

bool clk156_disable(i2caddr_t addr);
bool clk156_enable(i2caddr_t addr);

#ifdef __cplusplus
}
#endif

#endif
