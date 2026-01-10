// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static int ibk_file_loader_get_item_str (char *dst, size_t size, uint8_t index, const sbi_bank_t *sbi_bank)
{
  String_t s, t; // at most 8+1 bytes each
  char iname[8+1];
  char type[15+1];
  const struct sbi_bank_item_t *item = &sbi_bank->items[index];

  static const char _perc_voice[5][2+1] = { "BD", "SD", "TT", "TC", "HH" };

  // instrument name
  StrToString ((String *)&t, item->name, sizeof (t) - 1);
  s = asciiz_string ((String *)&t);
  t = CutStr ((String *)&s);
  s = ExpStrR ((String *)&t, sizeof (iname) - 1, ' ');
  StringToStr (iname, (String *)&s, sizeof (iname) - 1);

  if (item->ins.perc_voice == 0)
    strcpy (type, "MELODiC");
  else if (item->ins.perc_voice <= 5)
    snprintf (type, sizeof (type), "PERCUSSiON (%s)", _perc_voice[item->ins.perc_voice - 1]);
  else
    strcpy (type, "?");

  if (item->available)
    return snprintf (dst, size,
      " %3"PRIu8". ~%s~  %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"  %s",
      index + 1,
      iname,
      item->ins.fm_data.AM_VIB_EG_carrier,
      item->ins.fm_data.AM_VIB_EG_modulator,
      item->ins.fm_data.KSL_VOLUM_carrier,
      item->ins.fm_data.KSL_VOLUM_modulator,
      item->ins.fm_data.ATTCK_DEC_carrier,
      item->ins.fm_data.ATTCK_DEC_modulator,
      item->ins.fm_data.SUSTN_REL_carrier,
      item->ins.fm_data.SUSTN_REL_modulator,
      item->ins.fm_data.WAVEFORM_carrier,
      item->ins.fm_data.WAVEFORM_modulator,
      item->ins.fm_data.FEEDBACK_FM,
      type);
  else
    return snprintf (dst, size,
      " %3"PRIu8". %s  ---- ---- ---- ---- ---- --  -",
      index + 1,
      iname);
}
