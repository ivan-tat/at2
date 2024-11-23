// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define IDLE     0x0FFF
#define FINISHED 0xFFFF

void macro_poll_proc (void)
{
  DBG_ENTER ("macro_poll_proc");

  for (uint8_t chan = 0; chan < songdata.nm_tracks; chan++)
  {
    uint16_t finished_flag = (!keyoff_loop[chan]) ? FINISHED : IDLE;
    struct macro_table_t *mt = &macro_table[chan];
    tREGISTER_TABLE *im = &songdata.instr_macros[mt->fmreg_table - 1];

    if (mt->fmreg_table && speed)
    {
      if (mt->fmreg_duration > 1)
        mt->fmreg_duration--;
      else
      {
        mt->fmreg_count = 1;
        if (mt->fmreg_pos <= im->length)
        {
          if (im->loop_begin && im->loop_length)
          {
            if (mt->fmreg_pos == (im->loop_begin + im->loop_length - 1))
              mt->fmreg_pos = im->loop_begin;
            else if (mt->fmreg_pos < im->length)
              mt->fmreg_pos++;
            else
              mt->fmreg_pos = finished_flag;
          }
          else if (mt->fmreg_pos < im->length)
            mt->fmreg_pos++;
          else
            mt->fmreg_pos = finished_flag;
        }
        else
          mt->fmreg_pos = finished_flag;

        if ((freq_table[chan] & 0x2000)
            && im->keyoff_pos
            && (mt->fmreg_pos >= im->keyoff_pos))
          mt->fmreg_pos = IDLE;
        else if ((!(freq_table[chan] & 0x2000))
                 && mt->fmreg_pos && im->keyoff_pos
                 && ((mt->fmreg_pos < im->keyoff_pos) || (mt->fmreg_pos == IDLE)))
          mt->fmreg_pos = im->keyoff_pos;

        if (    mt->fmreg_pos
            && (mt->fmreg_pos != IDLE)
            && (mt->fmreg_pos != finished_flag))
        {
          mt->fmreg_duration = im->data[mt->fmreg_pos - 1].duration;
          if (mt->fmreg_duration)
          {
            // force KEY-ON with missing ADSR instrument data
            tREGISTER_TABLE_DEF *d = &im->data[mt->fmreg_pos - 1];
            tDIS_FMREG_COL *p = &songdata.dis_fmreg_col[mt->fmreg_table - 1];

            force_macro_keyon = false;
            if (mt->fmreg_pos == 1)
              if (is_ins_adsr_data_empty (voice_table[chan])
                  && !(   *p[0]
                       && *p[1]
                       && *p[2]
                       && *p[3]
                       && *p[12]
                       && *p[13]
                       && *p[14]
                       && *p[15]))
                force_macro_keyon = true;

            if (!*p[0])
              fmpar_table[chan].adsrw_mod.attck = d->fm_data.ATTCK_DEC_modulator >> 4;

            if (!*p[1])
              fmpar_table[chan].adsrw_mod.dec   = d->fm_data.ATTCK_DEC_modulator & 0x0F;

            if (!*p[2])
              fmpar_table[chan].adsrw_mod.sustn = d->fm_data.SUSTN_REL_modulator >> 4;

            if (!*p[3])
              fmpar_table[chan].adsrw_mod.rel   = d->fm_data.SUSTN_REL_modulator & 0x0F;

            if (!*p[4])
              fmpar_table[chan].adsrw_mod.wform = d->fm_data.WAVEFORM_modulator & 0x07;

            if (!*p[6])
              fmpar_table[chan].kslM            = d->fm_data.KSL_VOLUM_modulator >> 6;

            if (!*p[7])
              fmpar_table[chan].multipM         = d->fm_data.AM_VIB_EG_modulator & 0x0F;

            if (!*p[8])
              fmpar_table[chan].tremM           = d->fm_data.AM_VIB_EG_modulator >> 7;

            if (!*p[9])
              fmpar_table[chan].vibrM           = (d->fm_data.AM_VIB_EG_modulator >> 6) & 1;

            if (!*p[10])
              fmpar_table[chan].ksrM            = (d->fm_data.AM_VIB_EG_modulator >> 4) & 1;

            if (!*p[11])
              fmpar_table[chan].sustM           = (d->fm_data.AM_VIB_EG_modulator >> 5) & 1;

            if (!*p[12])
              fmpar_table[chan].adsrw_car.attck = d->fm_data.ATTCK_DEC_carrier >> 4;

            if (!*p[13])
              fmpar_table[chan].adsrw_car.dec   = d->fm_data.ATTCK_DEC_carrier & 0x0F;

            if (!*p[14])
              fmpar_table[chan].adsrw_car.sustn = d->fm_data.SUSTN_REL_carrier >> 4;

            if (!*p[15])
              fmpar_table[chan].adsrw_car.rel   = d->fm_data.SUSTN_REL_carrier & 0x0F;

            if (!*p[16])
              fmpar_table[chan].adsrw_car.wform = d->fm_data.WAVEFORM_carrier & 0x07;

            if (!*p[18])
              fmpar_table[chan].kslC            = d->fm_data.KSL_VOLUM_carrier >> 6;

            if (!*p[19])
              fmpar_table[chan].multipC         = d->fm_data.AM_VIB_EG_carrier & 0x0F;

            if (!*p[20])
              fmpar_table[chan].tremC           = d->fm_data.AM_VIB_EG_carrier >> 7;

            if (!*p[21])
              fmpar_table[chan].vibrC           = (d->fm_data.AM_VIB_EG_carrier >> 6) & 1;

            if (!*p[22])
              fmpar_table[chan].ksrC            = (d->fm_data.AM_VIB_EG_carrier >> 4) & 1;

            if (!*p[23])
              fmpar_table[chan].sustC           = (d->fm_data.AM_VIB_EG_carrier >> 5) & 1;

            if (!*p[24])
              fmpar_table[chan].connect         = d->fm_data.FEEDBACK_FM & 1;

            if (!*p[25])
              fmpar_table[chan].feedb           = (d->fm_data.FEEDBACK_FM >> 1) & 7;

            if (!*p[27])
              if (!pan_lock[chan])
                panning_table[chan] = d->panning;

            if (!*p[5])
              set_ins_volume (63 - (d->fm_data.KSL_VOLUM_modulator & 0x3F), BYTE_NULL, chan + 1);

            if (!*p[17])
              set_ins_volume (BYTE_NULL, 63 - (d->fm_data.KSL_VOLUM_carrier & 0x3F), chan + 1);

            update_modulator_adsrw (chan + 1);
            update_carrier_adsrw (chan + 1);
            update_fmpar (chan + 1);

            if (force_macro_keyon
                || !(    d->fm_data.FEEDBACK_FM
                     || (d->fm_data.FEEDBACK_FM != MACRO_NOTE_RETRIG_FLAG)))
            {
              if (!(is_4op_chan (chan + 1) || bit_test (_4op_tracks_hi, chan + 1)))
              {
                output_note (event_table[chan].note, event_table[chan].instr_def, chan + 1, false, true);
                if (is_4op_chan (chan + 1) && bit_test (_4op_tracks_lo, chan + 1))
                  init_macro_table (chan - 1 + 1, 0, voice_table[chan - 1], 0);
              }
            }
            else if (!(    d->fm_data.FEEDBACK_FM
                       || (d->fm_data.FEEDBACK_FM != MACRO_ENVELOPE_RESTART_FLAG)))
            {
              key_on (chan + 1);
              change_freq (chan + 1, freq_table[chan]);
            }
            else if (!(    d->fm_data.FEEDBACK_FM
                       || (d->fm_data.FEEDBACK_FM != MACRO_ZERO_FREQ_FLAG)))
            {
              if (freq_table[chan])
              {
                zero_fq_table[chan] = freq_table[chan];
                freq_table[chan] &= ~0x1FFF;
                change_freq (chan + 1, freq_table[chan]);
              }
            }
            else if (zero_fq_table[chan])
            {
              freq_table[chan] = zero_fq_table[chan];
              zero_fq_table[chan] = 0;
              change_freq (chan + 1, freq_table[chan]);
            }

            if (!*p[26])
            {
              if (d->freq_slide > 0)
                portamento_up (chan + 1, d->freq_slide, nFreq (12*8+1));
              else if (d->freq_slide < 0)
                portamento_down (chan + 1, -d->freq_slide, nFreq (0));
            }
          }
        }
      }
    }

    tARPEGGIO_TABLE *arp = &songdata.macro_table[mt->arpg_table - 1].arpeggio;
    if (mt->arpg_table && arp->speed)
    {
      if (mt->arpg_count == arp->speed)
      {
        mt->arpg_count = 1;
        if (mt->arpg_pos <= arp->length)
        {
          if (arp->loop_begin && arp->loop_length)
          {
            if (mt->arpg_pos == (arp->loop_begin + arp->loop_length - 1))
              mt->arpg_pos = arp->loop_begin;
            else if (mt->arpg_pos < arp->length)
              mt->arpg_pos++;
            else
              mt->arpg_pos = finished_flag;
          }
          else if (mt->arpg_pos < arp->length)
            mt->arpg_pos++;
          else
            mt->arpg_pos = finished_flag;
        }
        else
          mt->arpg_pos = finished_flag;

        if ((freq_table[chan] & 0x2000)
            && arp->keyoff_pos
            && (mt->arpg_pos >= arp->keyoff_pos))
          mt->arpg_pos = IDLE;
        else if ((!(freq_table[chan] & 0x2000))
                 && arp->keyoff_pos
                 && (   (mt->arpg_pos < arp->keyoff_pos)
                     || (mt->arpg_pos == IDLE)))
          mt->arpg_pos = arp->keyoff_pos;

        if (    mt->arpg_pos
            && (mt->arpg_pos != IDLE)
            && (mt->arpg_pos != finished_flag))
        {
          uint8_t a = arp->data[mt->arpg_pos - 1];
          uint16_t f;

          if (!a)
            f = nFreq (mt->arpg_note - 1);
          else if (a < (12*8+1))
            f = nFreq (max (mt->arpg_note + a, 12*8+1) - 1);
          else if ((a > 0x80) && (a <= (0x80 + 12*8+1))) // FIXME: `<=' => `<' ?
            f = nFreq (a - 0x80 - 1);
          else
            f = 0;

          if (f)
            change_frequency (chan + 1, f + (int8_t)(ins_parameter (event_table[chan].instr_def, 12)));
        }
      }
      else
        mt->arpg_count++;
    }

    tVIBRATO_TABLE *vib = &songdata.macro_table[mt->vib_table - 1].vibrato;
    if ((!mt->vib_paused) && mt->vib_table && vib->speed)
    {
      if (mt->vib_count == vib->speed)
      {
        if (mt->vib_delay)
          mt->vib_delay--;
        else
        {
          mt->vib_count = 1;
          if (mt->vib_pos <= vib->length)
          {
            if (vib->loop_begin && vib->loop_length)
            {
              if (mt->vib_pos == (vib->loop_begin + vib->loop_length - 1))
                mt->vib_pos = vib->loop_begin;
              else if (mt->vib_pos < vib->length)
                mt->vib_pos++;
              else
                mt->vib_pos = finished_flag;
            }
            else if (mt->vib_pos < vib->length)
              mt->vib_pos++;
            else
              mt->vib_pos = finished_flag;
          }
          else
             mt->vib_pos = finished_flag;

          if ((freq_table[chan] & 0x2000)
              && vib->keyoff_pos
              && (mt->vib_pos >= vib->keyoff_pos))
            mt->vib_pos = IDLE;
          else if ((!(freq_table[chan] & 0x2000))
                   && mt->vib_pos && vib->keyoff_pos
                   && ((mt->vib_pos < vib->keyoff_pos) || (mt->vib_pos == IDLE)))
            mt->vib_pos = vib->keyoff_pos;

          if (    mt->vib_pos
              && (mt->vib_pos != IDLE)
              && (mt->vib_pos != finished_flag))
          {
            if (vib->data[mt->vib_pos - 1] > 0)
              macro_vibrato__porta_up (chan + 1, vib->data[mt->vib_pos - 1]);
            else if (vib->data[mt->vib_pos - 1] < 0)
              macro_vibrato__porta_down (chan + 1, -vib->data[mt->vib_pos - 1]);
            else
              change_freq (chan + 1, mt->vib_freq);
          }
        }
      }
      else
        mt->vib_count++;
    }
  }

  DBG_LEAVE ();
}
