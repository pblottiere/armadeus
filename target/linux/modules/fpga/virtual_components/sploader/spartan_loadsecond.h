#ifndef SPARTAN_LOADSECOND_H
#define SPARTAN_LOADSECOND_H

/* Xilinx types
 *  *********************************************************************/
struct sp_vision_struct {
	uint32_t fpga_offset;
	int ip_addr;
	int ip_id;
};

typedef enum {			/* typedef Xilinx_iface */
	min_xilinx_iface_type,	/* low range check value */
	slave_serial,		/* serial data and external clock */
	master_serial,		/* serial data w/ internal clock (not used) */
	slave_parallel,		/* parallel data w/ external latch */
	jtag_mode,		/* jtag/tap serial (not used ) */
	master_selectmap,	/* master SelectMap (virtex2)           */
	slave_selectmap,	/* slave SelectMap (virtex2)            */
	max_xilinx_iface_type	/* insert all new types before this */
} Xilinx_iface;			/* end, typedef Xilinx_iface */

typedef enum {			/* typedef Xilinx_Family */
	min_xilinx_type,	/* low range check value */
	Xilinx_Spartan2,	/* Spartan-II Family */
	Xilinx_VirtexE,		/* Virtex-E Family */
	Xilinx_Virtex2,		/* Virtex2 Family */
	Xilinx_Spartan3,	/* Spartan-III Family */
	Xilinx_Spartan6,	/* Spartan-VI Family */
	max_xilinx_type		/* insert all new types before this */
} Xilinx_Family;		/* end, typedef Xilinx_Family */

typedef struct {		/* typedef Xilinx_desc */
	Xilinx_Family family;	/* part type */
	Xilinx_iface iface;	/* interface type */
	size_t size;		/* bytes of data part can accept */
	void *iface_fns;	/* interface function table */
	/* XXX:for second fpga configuration (throught first fpga) */
	uint32_t fpga_offset;	/* first fpga offset */
	void *ip_addr;		/* Address of spartan_selectmap IP */
	int ip_id;		/* Idendifiant of spartan_selectmap IP */
	/* end of XXX */
	int cookie;		/* implementation specific cookie */
	// ggm //
	const char *name;             /* instance name  */
	int num;                      /* instance number */
	int idnum;                    /* identity number */
	int idoffset;                 /* identity relative address */
	struct sploader_dev *sdev;    /* struct for main device structure */

} Xilinx_desc;			/* end, typedef Xilinx_desc */

typedef int (*Xilinx_pgm_fn) (Xilinx_desc *desc, int assert_pgm, int flush, int cookie);
typedef int (*Xilinx_init_fn) (Xilinx_desc *desc, int cookie);
typedef int (*Xilinx_err_fn) (Xilinx_desc *desc, int cookie);
typedef int (*Xilinx_done_fn) (Xilinx_desc *desc, int cookie);
typedef int (*Xilinx_clk_fn) (Xilinx_desc *desc, int assert_clk, int flush, int cookie);
typedef int (*Xilinx_cs_fn) (Xilinx_desc *desc, int assert_cs, int flush, int cookie);
typedef int (*Xilinx_wr_fn) (Xilinx_desc *desc, int assert_write, int flush, int cookie);
typedef int (*Xilinx_rdata_fn) (Xilinx_desc *desc, unsigned char *data, int cookie);
typedef int (*Xilinx_wdata_fn) (Xilinx_desc *desc, unsigned char data, int flush, int cookie);
typedef int (*Xilinx_busy_fn) (Xilinx_desc *desc, int cookie);
typedef int (*Xilinx_abort_fn) (Xilinx_desc *desc, int cookie);
typedef int (*Xilinx_pre_fn) (Xilinx_desc *desc, int cookie);
typedef int (*Xilinx_post_fn) (Xilinx_desc *desc, int cookie);

typedef struct {
	Xilinx_pre_fn pre;
	Xilinx_pgm_fn pgm;
	Xilinx_init_fn init;
	Xilinx_err_fn err;
	Xilinx_done_fn done;
	Xilinx_clk_fn clk;
	Xilinx_cs_fn cs;
	Xilinx_wr_fn wr;
	Xilinx_rdata_fn rdata;
	Xilinx_wdata_fn wdata;
	Xilinx_busy_fn busy;
	Xilinx_abort_fn abort;
	Xilinx_post_fn post;
	int relocated;
} Xilinx_Spartan6_Slave_SelectMap_fns;

#define FPGA_FAIL -1
#define FPGA_SUCCESS 0
#define FALSE                    (0)
#define TRUE                     (!FALSE)
#define SELECTMAP_CONFIG_REG        (desc->ip_addr+2)
#define SELECTMAP_STATUS_REG        (desc->ip_addr+4)
#define SELECTMAP_STATUS_BUSY       1

#define SELECTMAP_CONFIG_CLK                4
#define CONFIG_SYS_FPGA_WAIT            20000	/* 20 ms */
#define CONFIG_FPGA_DELAY()
#define SELECTMAP_STATUS_INIT_N     2
#define SELECTMAP_STATUS_DONE       0
#define SELECTMAP_DATA_REG  (desc->ip_addr+6)
#define SELECTMAP_CONFIG_RDWR_N             0
#define SELECTMAP_CONFIG_CSI_N              2
#define SELECTMAP_CONFIG_PROGRAM_N  1

int loadsecond(Xilinx_desc * desc/*, void *ip_addr, int ip_id*/);
int write_content(Xilinx_desc * desc, void *buf, size_t bsize);
int end_load(Xilinx_desc * desc);

int fpga_spartan6_pre_fn(Xilinx_desc *desc, int cookie);
int fpga_spartan6_pgm_fn(Xilinx_desc *desc, int assert_pgm, int flush, int cookie);
int fpga_spartan6_cs_fn(Xilinx_desc *desc, int assert_cs, int flush, int cookie);
int fpga_spartan6_init_fn(Xilinx_desc *desc, int cookie);
int fpga_spartan6_done_fn(Xilinx_desc *desc, int cookie);
int fpga_spartan6_clk_fn(Xilinx_desc *desc, int assert_clk, int flush, int cookie);
int fpga_spartan6_wr_fn(Xilinx_desc *desc, int assert_write, int flush, int cookie);
int fpga_spartan6_rdata_fn(Xilinx_desc *desc, unsigned char *data, int cookie);
int fpga_spartan6_wdata_fn(Xilinx_desc *desc, unsigned char data, int flush, int cookie);
int fpga_spartan6_abort_fn(Xilinx_desc *desc, int cookie);
int fpga_spartan6_busy_fn(Xilinx_desc *desc, int cookie);
int fpga_spartan6_post_fn(Xilinx_desc *desc, int cookie);

#endif				/*SPARTAN_LOADSECOND_H */
