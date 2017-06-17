/*
 * ngtcp2
 *
 * Copyright (c) 2017 ngtcp2 contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "ngtcp2_pkt_test.h"

#include <CUnit/CUnit.h>

#include "ngtcp2_pkt.h"

void test_ngtcp2_pkt_decode_hd_long(void) {
  ngtcp2_pkt_hd hd, nhd;
  uint8_t buf[256];
  ssize_t rv;

  ngtcp2_pkt_hd_init(&hd, NGTCP2_PKT_FLAG_LONG_FORM,
                     NGTCP2_PKT_VERSION_NEGOTIATION, 0xf1f2f3f4f5f6f7f8llu,
                     0xe1e2e3e4u, 0xd1d2d3d4u);

  rv = ngtcp2_pkt_encode_hd_long(buf, sizeof(buf), &hd);

  CU_ASSERT(NGTCP2_LONG_HEADERLEN == rv);

  rv = ngtcp2_pkt_decode_hd_long(&nhd, buf, NGTCP2_LONG_HEADERLEN);

  CU_ASSERT(NGTCP2_LONG_HEADERLEN == rv);
  CU_ASSERT(hd.flags == nhd.flags);
  CU_ASSERT(hd.conn_id == nhd.conn_id);
  CU_ASSERT(hd.pkt_num == nhd.pkt_num);
  CU_ASSERT(hd.version == nhd.version);
}

void test_ngtcp2_pkt_decode_hd_short(void) {
  ngtcp2_pkt_hd hd, nhd;
  uint8_t buf[256];
  ssize_t rv;
  size_t expectedlen;

  /* NGTCP2_PKT_03 */
  ngtcp2_pkt_hd_init(&hd, NGTCP2_PKT_FLAG_NONE, NGTCP2_PKT_03,
                     0xf1f2f3f4f5f6f7f8llu, 0xe1e2e3e4u, 0xd1d2d3d4u);

  expectedlen = 5;

  rv = ngtcp2_pkt_encode_hd_short(buf, sizeof(buf), &hd);

  CU_ASSERT((ssize_t)expectedlen == rv);

  rv = ngtcp2_pkt_decode_hd_short(&nhd, buf, expectedlen);

  CU_ASSERT((ssize_t)expectedlen == rv);
  CU_ASSERT(hd.flags == nhd.flags);
  CU_ASSERT(0 == nhd.conn_id);
  CU_ASSERT(hd.pkt_num == nhd.pkt_num);
  CU_ASSERT(0 == nhd.version);

  /* NGTCP2_PKT_02 */
  ngtcp2_pkt_hd_init(&hd, NGTCP2_PKT_FLAG_NONE, NGTCP2_PKT_02,
                     0xf1f2f3f4f5f6f7f8llu, 0xe1e2e3e4u, 0xd1d2d3d4u);

  expectedlen = 3;

  rv = ngtcp2_pkt_encode_hd_short(buf, sizeof(buf), &hd);

  CU_ASSERT((ssize_t)expectedlen == rv);

  rv = ngtcp2_pkt_decode_hd_short(&nhd, buf, expectedlen);

  CU_ASSERT((ssize_t)expectedlen == rv);
  CU_ASSERT(hd.flags == nhd.flags);
  CU_ASSERT(0 == nhd.conn_id);
  CU_ASSERT((hd.pkt_num & 0xffff) == nhd.pkt_num);
  CU_ASSERT(0 == nhd.version);

  /* NGTCP2_PKT_01 */
  ngtcp2_pkt_hd_init(&hd, NGTCP2_PKT_FLAG_NONE, NGTCP2_PKT_01,
                     0xf1f2f3f4f5f6f7f8llu, 0xe1e2e3e4u, 0xd1d2d3d4u);

  expectedlen = 2;

  rv = ngtcp2_pkt_encode_hd_short(buf, sizeof(buf), &hd);

  CU_ASSERT((ssize_t)expectedlen == rv);

  rv = ngtcp2_pkt_decode_hd_short(&nhd, buf, expectedlen);

  CU_ASSERT((ssize_t)expectedlen == rv);
  CU_ASSERT(hd.flags == nhd.flags);
  CU_ASSERT(0 == nhd.conn_id);
  CU_ASSERT((hd.pkt_num & 0xff) == nhd.pkt_num);
  CU_ASSERT(0 == nhd.version);

  /* With connection ID, and Key Phase */
  ngtcp2_pkt_hd_init(&hd, NGTCP2_PKT_FLAG_CONN_ID | NGTCP2_PKT_FLAG_KEY_PHASE,
                     NGTCP2_PKT_03, 0xf1f2f3f4f5f6f7f8llu, 0xe1e2e3e4u,
                     0xd1d2d3d4u);

  expectedlen = 13;

  rv = ngtcp2_pkt_encode_hd_short(buf, sizeof(buf), &hd);

  CU_ASSERT((ssize_t)expectedlen == rv);

  rv = ngtcp2_pkt_decode_hd_short(&nhd, buf, expectedlen);

  CU_ASSERT((ssize_t)expectedlen == rv);
  CU_ASSERT(hd.flags == nhd.flags);
  CU_ASSERT(hd.conn_id == nhd.conn_id);
  CU_ASSERT(hd.pkt_num == nhd.pkt_num);
  CU_ASSERT(0 == nhd.version);
}
