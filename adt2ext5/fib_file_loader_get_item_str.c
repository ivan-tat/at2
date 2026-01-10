// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// index: 0..fin_bank->capacity-1
static int fib_file_loader_get_item_str (char *dst, size_t size, uint16_t index, const fin_bank_t *fin_bank)
{
  String_t s, t;
/*
  char dname[8+1];
*/
  char iname[FIN_INS_NAME_LEN+1];
  char type[5+1];
  const struct fin_bank_item_t *item = &fin_bank->items[index];

  static const char _perc_voice[5][2+1] = { "BD", "SD", "TT", "TC", "HH" };

/*
  // DOS base file name (w/o extension)
  t = BaseNameOnly (StrToString ((String *)&s, item->dname, sizeof (item->dname) - 1));
  s = CutStr ((String *)&t);
  if (s.len > sizeof (dname) - 1) s.len = sizeof (dname) - 1;
  t = Upper ((String *)&s);
  s = ExpStrR ((String *)&t, sizeof (dname) - 1, ' ');
  StringToStr (dname, (String *)&s, sizeof (dname) - 1);
*/

  // instrument name
  t = CutStr (StrToString ((String *)&s, item->iname, sizeof (item->iname) - 1));
  s = ExpStrR ((String *)&t, sizeof (iname) - 1, ' ');
  StringToStr (iname, (String *)&s, sizeof (iname) - 1);

  // type
  if (item->ins.perc_voice == 0)
    strcpy (type, "MEL.");
  else if (item->ins.perc_voice <= 5)
    snprintf (type, sizeof (type), "P(%s)", _perc_voice[item->ins.perc_voice - 1]);
  else
    strcpy (type, "?");

  // 1st character is always space if index < 9999
  if (item->available)
    return snprintf (dst, size,
      "%5"PRIu16". %s%s%s  %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"%02"PRIX8" %02"PRIX8"  %s",
      index + 1,
      item->available ? "~" : "",
      iname,
      item->available ? "~" : "",
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
      "%5"PRIu16". %s  ---- ---- ---- ---- ---- --  -",
      index + 1,
      iname);
}
