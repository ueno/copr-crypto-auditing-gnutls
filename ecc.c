/*
 * Copyright (C) 2011-2012 Free Software Foundation, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#include <gnutls_int.h>
#include <algorithms.h>
#include <gnutls_errors.h>
#include <x509/common.h>


/* Supported ECC curves
 */

static const gnutls_ecc_curve_entry_st ecc_curves[] = {
  {
    .name = "SECP256R1", 
    .oid = "1.2.840.10045.3.1.7",
    .id = GNUTLS_ECC_CURVE_SECP256R1,
    .tls_id = 23,
    .size = 32,
    .prime = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF",
    .A = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC",
    .B = "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B",
    .order = "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551",
    .Gx = "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296",
    .Gy = "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5",
  },
  {
    .name = "SECP384R1",
    .oid = "1.3.132.0.34",
    .id = GNUTLS_ECC_CURVE_SECP384R1,
    .tls_id = 24,
    .size = 48,
    .prime = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF",
    .A = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFC",
    .B = "B3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A2ED19D2A85C8EDD3EC2AEF",
    .order = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248B0A77AECEC196ACCC52973",
    .Gx = "AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF55296C3A545E3872760AB7",
    .Gy = "3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1CE1D7E819D7A431D7C90EA0E5F"
  },
  {0, 0, 0}
};

#define GNUTLS_ECC_CURVE_LOOP(b) \
	{ const gnutls_ecc_curve_entry_st *p; \
                for(p = ecc_curves; p->name != NULL; p++) { b ; } }


/* Returns the TLS id of the given curve
 */
int
_gnutls_tls_id_to_ecc_curve (int num)
{
  gnutls_ecc_curve_t ret = GNUTLS_ECC_CURVE_INVALID;

  GNUTLS_ECC_CURVE_LOOP (
  if (p->tls_id == num) 
    {
      ret = p->id;
      break;
    }
  );
  
  return ret;
}

/**
 * gnutls_ecc_curve_list:
 *
 * Get the list of supported elliptic curves.
 *
 * This function is not thread safe.
 *
 * Returns: Return a (0)-terminated list of #gnutls_ecc_curve_t
 *   integers indicating the available curves.
 **/
const gnutls_ecc_curve_t *
gnutls_ecc_curve_list (void)
{
static gnutls_ecc_curve_t supported_curves[MAX_ALGOS] = { 0 };

  if (supported_curves[0] == 0)
    {
      int i = 0;

      GNUTLS_ECC_CURVE_LOOP ( 
        supported_curves[i++]=p->id;
      );
      supported_curves[i++]=0;
    }

  return supported_curves;
}

/* Maps numbers to TLS NamedCurve IDs (RFC4492).
 * Returns a negative number on error.
 */
int
_gnutls_ecc_curve_get_tls_id (gnutls_ecc_curve_t supported_ecc)
{
  int ret = GNUTLS_E_RECEIVED_ILLEGAL_PARAMETER;

  GNUTLS_ECC_CURVE_LOOP (
  if (p->id == supported_ecc) 
    {
      ret = p->tls_id;
      break;
    }
  );
  
  return ret;
}

/*-
 * _gnutls_oid_to_ecc_curve:
 * @oid: is a curve's OID
 *
 * Returns: return a #gnutls_ecc_curve_t value corresponding to
 *   the specified OID, or %GNUTLS_ECC_CURVE_INVALID on error.
 -*/
gnutls_ecc_curve_t _gnutls_oid_to_ecc_curve (const char* oid)
{
  gnutls_ecc_curve_t ret = GNUTLS_ECC_CURVE_INVALID;

  GNUTLS_ECC_CURVE_LOOP (
  if (strcasecmp (p->oid, oid) == 0) 
    {
      ret = p->id;
      break;
    }
  );

  return ret;
}

/*-
 * _gnutls_ecc_curve_get_id:
 * @name: is a curve name
 *
 * The names are compared in a case insensitive way.
 *
 * Returns: return a #gnutls_ecc_curve_t value corresponding to
 *   the specified curve, or %GNUTLS_ECC_CURVE_INVALID on error.
 -*/
gnutls_ecc_curve_t
_gnutls_ecc_curve_get_id (const char *name)
{
  gnutls_ecc_curve_t ret = GNUTLS_ECC_CURVE_INVALID;

  GNUTLS_ECC_CURVE_LOOP (
  if (strcasecmp (p->name, name) == 0) 
    {
      ret = p->id;
      break;
    }
  );

  return ret;
}

/*-
 * _gnutls_ecc_bits_to_curve:
 * @bits: is a security parameter in bits
 *
 * Returns: return a #gnutls_ecc_curve_t value corresponding to
 *   the specified bit length, or %GNUTLS_ECC_CURVE_INVALID on error.
 -*/
gnutls_ecc_curve_t
_gnutls_ecc_bits_to_curve (int bits)
{
  gnutls_ecc_curve_t ret = GNUTLS_ECC_CURVE_SECP224R1;

  GNUTLS_ECC_CURVE_LOOP (
    if (8*p->size >= bits)
      {
        ret = p->id;
        break;
      }
  );

  return ret;
}

/**
 * gnutls_ecc_curve_get_name:
 * @curve: is an ECC curve
 *
 * Convert a #gnutls_ecc_curve_t value to a string.
 *
 * Returns: a string that contains the name of the specified
 *   curve or %NULL.
 *
 * Since: 3.0
 **/
const char *
gnutls_ecc_curve_get_name (gnutls_ecc_curve_t curve)
{
  const char *ret = NULL;

  GNUTLS_ECC_CURVE_LOOP(
    if (p->id == curve)
      {
        ret = p->name;
        break;
      }
  );

  return ret;
}

/*-
 * _gnutls_ecc_curve_get_oid:
 * @curve: is an ECC curve
 *
 * Convert a #gnutls_ecc_curve_t value to a string.
 *
 * Returns: a string that contains the name of the specified
 *   curve or %NULL.
 -*/
const char *
_gnutls_ecc_curve_get_oid (gnutls_ecc_curve_t curve)
{
  const char *ret = NULL;

  GNUTLS_ECC_CURVE_LOOP(
    if (p->id == curve)
      {
        ret = p->oid;
        break;
      }
  );

  return ret;
}

/*-
 * _gnutls_ecc_curve_get_params:
 * @curve: is an ECC curve
 *
 * Returns the information on a curve.
 *
 * Returns: a pointer to #gnutls_ecc_curve_entry_st or %NULL.
 -*/
const gnutls_ecc_curve_entry_st *
_gnutls_ecc_curve_get_params (gnutls_ecc_curve_t curve)
{
  const gnutls_ecc_curve_entry_st *ret = NULL;

  GNUTLS_ECC_CURVE_LOOP(
    if (p->id == curve)
      {
        ret = p;
        break;
      }
  );

  return ret;
}

/**
 * gnutls_ecc_curve_get_size:
 * @curve: is an ECC curve
 *
 * Returns the size in bytes of the curve.
 *
 * Returns: a the size or (0).
 *
 * Since: 3.0
 **/
int gnutls_ecc_curve_get_size (gnutls_ecc_curve_t curve)
{
  int ret = 0;

  GNUTLS_ECC_CURVE_LOOP(
    if (p->id == curve)
      {
        ret = p->size;
        break;
      }
  );

  return ret;
}
