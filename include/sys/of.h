/*-
 * Copyright (c) 2019-2021 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef	_SYS_OF_H_
#define	_SYS_OF_H_

bool mdx_of_is_compatible(mdx_device_t dev, const char *compatstr);
void mdx_of_install_dtbp(void *dtbp);
int mdx_of_check_header(void);
int mdx_of_find_first_compatible(const char *compat);
int mdx_of_get_reg(mdx_device_t dev, int index,
    size_t *addr, size_t *size);
int mdx_of_intc_offset(int offset);
int mdx_of_dev_get_prop32(mdx_device_t dev, const char *propname,
    uint32_t *res, int *len);
int mdx_of_setup_intr(mdx_device_t dev, int irq,
    void (*handler)(void *arg, int irq), void *arg);
int mdx_of_ld32(const void *regp);
void * mdx_of_get_dtbp(void);
bool mdx_of_node_is_enabled(int offset);
bool mdx_of_node_is_compatible(int offset, const char *check);
const char * mdx_of_dev_get_prop(mdx_device_t dev, const char *propname,
    int *len);
int mdx_of_node_offset_by_phandle(uint32_t phandle);
int mdx_of_parent_offset(int nodeoffset);
int mdx_of_get_prop32(int nodeoffset, const char *propname,
    uint32_t *res, int *len);
int mdx_of_get_cells(int offset, uint32_t *addr, uint32_t *size);
int mdx_of_offset_by_compatible(int offset, const char *compatible);
int mdx_of_chosen_path_offset(void);
bool mdx_of_dev_uart_chosen(mdx_device_t dev);
int mdx_of_probe_and_attach(int offset, struct mdx_driver *dri,
    mdx_device_t *dev0);
int mdx_of_node_by_path(const char *path);

#endif /* _SYS_OF_H_ */
