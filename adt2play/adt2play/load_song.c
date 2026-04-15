// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ int8_t load_song (const String *fname, char **error)
{
  int8_t result;

  result = a2m_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_A2M_0;
  else if (result ==  1) return MODNAME_A2M_1;
  else if (result <= -3) return result;

  result = a2t_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_A2T_0;
  else if (result ==  1) return MODNAME_A2T_1;
  else if (result <= -3) return result;

  result = amd_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_AMD_0;
  else if (result ==  1) return MODNAME_AMD_1;
  else if (result <= -3) return result;

  result = cff_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_CFF;
  else if (result <= -3) return result;

  result = dfm_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_DFM;
  else if (result <= -3) return result;

  result = mtk_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_MTK;
  else if (result <= -3) return result;

  result = rad_file_loader (fname, NULL, NULL, NULL, error);
  if      (result ==  0) return MODNAME_RAD;
  else if (result <= -3) return result;

  result = s3m_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_S3M;
  else if (result <= -3) return result;

  result = fmk_file_loader (fname, NULL, NULL, NULL, error);
  if      (result ==  0) return MODNAME_FMK;
  else if (result <= -3) return result;

  result = sat_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_SAT_0;
  else if (result ==  1) return MODNAME_SAT_1;
  else if (result <= -3) return result;

  result = hsc_file_loader (fname, NULL, NULL, error);
  if      (result ==  0) return MODNAME_HSC;

  return result;
}
