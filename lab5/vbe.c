#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	if (lm_init() == NULL) {
		printf("Nao foi possivel inicializar memoria.\n");
		return 1;
	}

	mmap_t mmap;

	struct reg86u r;

	if (lm_alloc(sizeof((*vmi_p)), &mmap) == NULL)
		return 1;

	else {

		r.u.b.intno = VIDEO_CARD_INTERRUPT_VECTOR;
		r.u.b.ah = VBE_CALL_FUNCTION;
		r.u.b.al = VBE_RETURN_INFO_FUNCTION;
		r.u.w.cx = mode;
		r.u.w.es = PB2BASE(mmap.phys);
		r.u.w.di = PB2OFF(mmap.phys);

		if (sys_int86(&r) != OK) {

			lm_free(&mmap);
			return 1;
		}

		else if (r.u.w.ax
				== (VBE_CALLED_FUNCTION_SUPPORTED | VBE_FUNCTION_COMPLETED)) {

			*vmi_p = *(vbe_mode_info_t*) mmap.virtual;
			lm_free(&mmap);
			return 0;

		}

	}
	return 1;
}

int vbe_get_info_block(vbe_info_block_t *vib_p){


	if (lm_init() == NULL) {
		printf("Nao foi possivel inicializar memoria.\n");
		return 1;
	}

	mmap_t mmap;

	struct reg86u r;

	if (lm_alloc(sizeof((*vib_p)), &mmap) == NULL)

		return 1;

	else {

		r.u.b.intno = VIDEO_CARD_INTERRUPT_VECTOR;
		r.u.b.ah = VBE_CALL_FUNCTION;
		r.u.b.al = VBE_INFO_BLOCK_FUNCTION;
		r.u.w.es = PB2BASE(mmap.phys);
		r.u.w.di = PB2OFF(mmap.phys);

		if (sys_int86(&r) != OK) {

			lm_free(&mmap);
			return 1;
		}

		else if (r.u.w.ax
				== (VBE_CALLED_FUNCTION_SUPPORTED | VBE_FUNCTION_COMPLETED)) {

			*vib_p = *(vbe_info_block_t*) mmap.virtual;

		}

	}

	lm_free(&mmap);


	return 0;

}

