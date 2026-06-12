// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_fm_data_cff (tFM_INST_DATA *dst, ins_fm_data_cff_t *src)
{
  dst->AM_VIB_EG_modulator = src->AM_Vibrato_EG_modulator;
  dst->AM_VIB_EG_carrier   = src->AM_Vibrato_EG_carrier;
  dst->KSL_VOLUM_modulator = src->KSL_Volume_modulator;
  dst->KSL_VOLUM_carrier   = src->KSL_Volume_carrier;
  dst->ATTCK_DEC_modulator = src->Attack_Decay_modulator;
  dst->ATTCK_DEC_carrier   = src->Attack_Decay_carrier;
  dst->SUSTN_REL_modulator = src->Sustain_Release_modulator;
  dst->SUSTN_REL_carrier   = src->Sustain_Release_carrier;
  dst->WAVEFORM_modulator  = src->WaveForm_modulator & 3;
  dst->WAVEFORM_carrier    = src->WaveForm_carrier & 3;
  dst->FEEDBACK_FM         = src->Feedback_FM & 0x0F;
}
