// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_fm_data_a2m_v1 (tFM_INST_DATA *dst, ins_fm_data_a2m_v1_t *src)
{
  dst->AM_VIB_EG_modulator = src->AM_Vibrato_EG[0];
  dst->AM_VIB_EG_carrier   = src->AM_Vibrato_EG[1];
  dst->KSL_VOLUM_modulator = src->KSL_Volume[0];
  dst->KSL_VOLUM_carrier   = src->KSL_Volume[1];
  dst->ATTCK_DEC_modulator = src->Attack_Decay[0];
  dst->ATTCK_DEC_carrier   = src->Attack_Decay[1];
  dst->SUSTN_REL_modulator = src->Sustain_Release[0];
  dst->SUSTN_REL_carrier   = src->Sustain_Release[1];
  dst->WAVEFORM_modulator  = src->WaveForm[0];
  dst->WAVEFORM_carrier    = src->WaveForm[1];
  dst->FEEDBACK_FM         = src->Feedback_FM;
}
