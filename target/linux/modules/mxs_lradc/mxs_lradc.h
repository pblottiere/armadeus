/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _ADC_DEF_H_
#define _ADC_DEF_H_

/* For the leds-gpio driver */
struct mxs_lradc_info {
	const char *name;
	int		channel;	/* LRADC channel */
	int		mux_pad;	/* LRADC muxed pad */
	int		enable_div2;	/* enable for signal up to VDDIO -50mV*/
	int		samples;	/* oversampling factor */
};

struct mxs_lradc_platform_data {
	int	num_lradc;
	struct mxs_lradc_info *lradc;
};

#endif	/* _ADC_DEF_H_*/
