// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_ins_fm_data_a2m_v1 (ins_fm_data_a2m_v1_t *dst, tFM_INST_DATA *src)
{
  dst->AM_Vibrato_EG[0]   = src->AM_VIB_EG_modulator;
  dst->AM_Vibrato_EG[1]   = src->AM_VIB_EG_carrier;
  dst->KSL_Volume[0]      = src->KSL_VOLUM_modulator;
  dst->KSL_Volume[1]      = src->KSL_VOLUM_carrier;
  dst->Attack_Decay[0]    = src->ATTCK_DEC_modulator;
  dst->Attack_Decay[1]    = src->ATTCK_DEC_carrier;
  dst->Sustain_Release[0] = src->SUSTN_REL_modulator;
  dst->Sustain_Release[1] = src->SUSTN_REL_carrier;
  dst->WaveForm[0]        = src->WAVEFORM_modulator;
  dst->WaveForm[1]        = src->WAVEFORM_carrier;
  dst->Feedback_FM        = src->FEEDBACK_FM;
}
