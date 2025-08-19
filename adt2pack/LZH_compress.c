// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Encoder state
struct LZHEncoderState_t
{
  uint16_t WIN_SIZE;
  uint16_t DIC_SIZE;
  uint16_t l_tree[2*(NC-1)+1], r_tree[2*(NC-1)+1];
  uint8_t  p_len[NT];
  uint8_t  c_len[NC];
  uint16_t heap[NC+1];
  uint16_t len_count[17];
  uint16_t c_freq[2*(NC-1)+1];
  uint16_t p_freq[2*(NP-1)+1];
  uint16_t t_freq[2*(NT-1)+1];
  uint16_t c_code[NC];
  uint16_t p_code[NT];
  int16_t  *freq, *sort_ptr, *pos_ptr;
  uint8_t  *buf, *len, *stream, *child_count, *level;
  int16_t  *parent, *previous, *next;
  int16_t  bits, heap_size, remain;
  int16_t  match_len;
  uint16_t sbit_buf, bit_count;
  uint16_t depth, c_pos, pos, out_pos, match_pos;
  uint16_t out_mask, avail;
  const uint8_t *input_buffer;
  uint8_t  *output_buffer;
  uint32_t input_buffer_idx, output_buffer_idx;
  uint32_t input_buffer_size;
};

// LZHEncoderState_t / TLZHEncoderState

const struct
{
  uint32_t
     WIN_SIZE,
     DIC_SIZE,
     l_tree, r_tree,
     p_len,
     c_len,
     heap,
     len_count,
     c_freq,
     p_freq,
     t_freq,
     c_code,
     p_code,
     freq, sort_ptr, pos_ptr,
     buf, len, stream, child_count, level,
     parent, previous, next,
     bits, heap_size, remain,
     match_len,
     sbit_buf, bit_count,
     depth, c_pos, pos, out_pos, match_pos,
     out_mask, avail,
     input_buffer,
     output_buffer,
     input_buffer_idx, output_buffer_idx,
     input_buffer_size;
} struct_TLZHEncoderState =
{
  _put_memb_off (struct LZHEncoderState_t, WIN_SIZE),
  _put_memb_off (struct LZHEncoderState_t, DIC_SIZE),
  _put_memb_off (struct LZHEncoderState_t, l_tree),
  _put_memb_off (struct LZHEncoderState_t, r_tree),
  _put_memb_off (struct LZHEncoderState_t, p_len),
  _put_memb_off (struct LZHEncoderState_t, c_len),
  _put_memb_off (struct LZHEncoderState_t, heap),
  _put_memb_off (struct LZHEncoderState_t, len_count),
  _put_memb_off (struct LZHEncoderState_t, c_freq),
  _put_memb_off (struct LZHEncoderState_t, p_freq),
  _put_memb_off (struct LZHEncoderState_t, t_freq),
  _put_memb_off (struct LZHEncoderState_t, c_code),
  _put_memb_off (struct LZHEncoderState_t, p_code),
  _put_memb_off (struct LZHEncoderState_t, freq),
  _put_memb_off (struct LZHEncoderState_t, sort_ptr),
  _put_memb_off (struct LZHEncoderState_t, pos_ptr),
  _put_memb_off (struct LZHEncoderState_t, buf),
  _put_memb_off (struct LZHEncoderState_t, len),
  _put_memb_off (struct LZHEncoderState_t, stream),
  _put_memb_off (struct LZHEncoderState_t, child_count),
  _put_memb_off (struct LZHEncoderState_t, level),
  _put_memb_off (struct LZHEncoderState_t, parent),
  _put_memb_off (struct LZHEncoderState_t, previous),
  _put_memb_off (struct LZHEncoderState_t, next),
  _put_memb_off (struct LZHEncoderState_t, bits),
  _put_memb_off (struct LZHEncoderState_t, heap_size),
  _put_memb_off (struct LZHEncoderState_t, remain),
  _put_memb_off (struct LZHEncoderState_t, match_len),
  _put_memb_off (struct LZHEncoderState_t, sbit_buf),
  _put_memb_off (struct LZHEncoderState_t, bit_count),
  _put_memb_off (struct LZHEncoderState_t, depth),
  _put_memb_off (struct LZHEncoderState_t, c_pos),
  _put_memb_off (struct LZHEncoderState_t, pos),
  _put_memb_off (struct LZHEncoderState_t, out_pos),
  _put_memb_off (struct LZHEncoderState_t, match_pos),
  _put_memb_off (struct LZHEncoderState_t, out_mask),
  _put_memb_off (struct LZHEncoderState_t, avail),
  _put_memb_off (struct LZHEncoderState_t, input_buffer),
  _put_memb_off (struct LZHEncoderState_t, output_buffer),
  _put_memb_off (struct LZHEncoderState_t, input_buffer_idx),
  _put_memb_off (struct LZHEncoderState_t, output_buffer_idx),
  _put_memb_off (struct LZHEncoderState_t, input_buffer_size)
};

const uint32_t struct_TLZHEncoderState_size = sizeof (struct LZHEncoderState_t);

//static
 uint16_t ReadDataBlock (struct LZHEncoderState_t *es, void *ptr, uint16_t size)
#if 0
{
  uint16_t result = es->input_buffer_size - es->input_buffer_idx <= size
                  ? es->input_buffer_size - es->input_buffer_idx
                  : size;
  memmove (ptr, &es->input_buffer[es->input_buffer_idx], result);
  es->input_buffer_idx += result;
#if !ADT2PLAY
  if (!really_no_status_refresh)
    show_progress2 (es->input_buffer_idx, 3);
#endif // !ADT2PLAY
  return result;
}
#else
;
#endif

//static
 void PutBits (struct LZHEncoderState_t *es, int16_t bits, uint16_t xbits)
#if 0
{
  if (bits < es->bit_count)
  {
    es->bit_count -= bits;
    es->sbit_buf |= xbits << es->bit_count;
  }
  else
  {
    bits -= es->bit_count;
    es->output_buffer[es->output_buffer_idx++] = es->sbit_buf | (xbits >> bits);
    if (bits < 8)
    {
      es->bit_count = 8 - bits;
      es->sbit_buf = xbits << es->bit_count;
    }
    else
    {
      es->output_buffer[es->output_buffer_idx++] = xbits >> (bits - 8);
      es->bit_count = 16 - bits;
      es->sbit_buf = xbits << es->bit_count;
    }
  }
}
#else
;
#endif

//static
 void CountLen (struct LZHEncoderState_t *es, int16_t idx)
#if 0
{
  if (idx < es->bits)
    es->len_count[es->depth <= 16 ? es->depth : 16]++;
  else
  {
    es->depth++;
    CountLen (es, es->l_tree[idx]);
    CountLen (es, es->r_tree[idx]);
    es->depth--;
  }
}
#else
;
#endif

//static
 void MakeLen (struct LZHEncoderState_t *es, int16_t root)
#if 0
{
  uint16_t sum = 0;

  memset (es->len_count, 0, sizeof (es->len_count));
  CountLen (es, root);
  for (int i = 16; i; i--)
    sum += es->len_count[i] << (16 - i);
  for (; sum != 0; sum--)
  {
    es->len_count[16]--;
    for (int i = 15; i; i--)
      if (es->len_count[i] != 0)
      {
        es->len_count[i]--;
        es->len_count[i + 1] += 2;
        break;
      }
  }
  for (int i = 16; i; i--)
    for (int16_t j = es->len_count[i] - 1; j >= 0; j--)
      es->len[*es->sort_ptr++] = i;
}
#else
;
#endif

//static
 void DownHeap (struct LZHEncoderState_t *es, int16_t idx)
#if 0
{
  int16_t i = es->heap[idx];

  for (int16_t j = idx << 1; j <= es->heap_size; j <<= 1)
  {
    if ((j < es->heap_size) && (es->freq[es->heap[j]] > es->freq[es->heap[j + 1]])) j++;
    if (es->freq[i] <= es->freq[es->heap[j]]) break;
    es->heap[idx] = es->heap[j];
    idx = j;
  }
  es->heap[idx] = i;
}
#else
;
#endif

//static
 void MakeCode (struct LZHEncoderState_t *es, int16_t bits, const uint8_t *len, int16_t *code)
#if 0
{
  uint16_t start[18];

  start[1] = 0;
  for (int i = 1; i < 17; i++)
    start[i + 1] = (start[i] + es->len_count[i]) << 1;
  for (int i = 0; i < bits; i++)
    code[i] = start[len[i]]++;
}
#else
;
#endif

//static
 int16_t MakeTree (struct LZHEncoderState_t *es,
                         int16_t n_par,
                         int16_t *f_par,
                         uint8_t *l_par,
                         int16_t *c_par)
#if 0
{
  int16_t result, avail;

  es->bits = n_par;
  es->freq = f_par;
  es->len = l_par;
  avail = es->bits;
  es->heap_size = 0;
  es->heap[1] = 0;

  for (int i = 0; i < es->bits; i++)
  {
    es->len[i] = 0;
    if (es->freq[i] != 0)
      es->heap[++es->heap_size] = i;
  }

  if (es->heap_size < 2)
  {
    c_par[es->heap[1]] = 0;
    return es->heap[1]; //MakeTree
  }

  for (int i = es->heap_size / 2; i; i--)
    DownHeap (es, i);

  es->sort_ptr = c_par;
  do
  {
    int16_t i = es->heap[1];
    int16_t j;

    if (i < es->bits) *es->sort_ptr++ = i;
    es->heap[1] = es->heap[es->heap_size--];
    DownHeap (es, 1);
    j = es->heap[1];
    if (j < es->bits) *es->sort_ptr++ = j;
    result = avail++;
    es->freq[result] = es->freq[i] + es->freq[j];
    es->heap[1] = result;
    DownHeap (es, 1);
    es->l_tree[result] = i;
    es->r_tree[result] = j;
  } while (es->heap_size > 1);

  es->sort_ptr = c_par;
  MakeLen (es, result);
  MakeCode (es, n_par, l_par, c_par);
  return result; //EXIT //MakeTree
}
#else
;
#endif

//static
 void CountFreq (struct LZHEncoderState_t *es)
#if 0
{
  int16_t bits;

  for (int i = 0; i < NT; es->t_freq[i++] = 0);
  for (bits = NC; (bits > 0) && (es->c_len[bits - 1] == 0); bits--);
  for (int i = 0; i < bits;)
  {
    int j = es->c_len[i++];
    if (j == 0)
    {
      int16_t count = 1;
      for (; (i < bits) && (es->c_len[i] == 0); i++) count++;
      if (count <= 2)
        es->t_freq[0] += count;
      else if (count <= 18)
        es->t_freq[1]++;
      else if (count == 19)
      {
        es->t_freq[0]++;
        es->t_freq[1]++;
      }
      else
        es->t_freq[2]++;
    }
    else
      es->t_freq[j + 2]++;
  }
}
#else
;
#endif

//static
 void WritePtrLen (struct LZHEncoderState_t *es, int16_t bits, int16_t n_bit, int16_t s_bit)
#if 0
{
  for (; (bits > 0) && (es->p_len[bits - 1] == 0); bits--);
  PutBits (es, n_bit, bits);
  for (int16_t i = 0; i < bits;)
  {
    int16_t j = es->p_len[i++];
    if (j <= 6)
      PutBits (es, 3, j);
    else
    {
      j -= 3;
      PutBits (es, j, (1 << j) - 2);
    }
    if (i == s_bit)
    {
      for (; (i < 6) && (es->p_len[i] == 0); i++);
      PutBits (es, 2, (i - 3) & 3);
    }
  }
}
#else
;
#endif

//static
 void WriteCharLen (struct LZHEncoderState_t *es)
#if 0
{
  int16_t bits = NC;

  for (; (bits > 0) && (es->c_len[bits - 1] == 0); bits--);
  PutBits (es, CBIT, bits);
  for (int16_t i = 0; i < bits;)
  {
    int16_t j = es->c_len[i++];
    if (j == 0)
    {
      int16_t count = 1;
      for (; (i < bits) && (es->c_len[i] == 0); i++) count++;
      if (count <= 2)
      {
        for (j = 0; j < count; j++)
          PutBits (es, es->p_len[0], es->p_code[0]);
      }
      else if (count <= 18)
      {
        PutBits (es, es->p_len[1], es->p_code[1]);
        PutBits (es, 4, count - 3);
      }
      else if (count == 19)
      {
        PutBits (es, es->p_len[0], es->p_code[0]);
        PutBits (es, es->p_len[1], es->p_code[1]);
        PutBits (es, 4, 15);
      }
      else
      {
        PutBits (es, es->p_len[2], es->p_code[2]);
        PutBits (es, CBIT, count - 20);
      }
    }
    else
      PutBits (es, es->p_len[j + 2], es->p_code[j + 2]);
  }
}
#else
;
#endif

//static
 void EncodePtr (struct LZHEncoderState_t *es, uint16_t ptr)
#if 0
{
  uint16_t i = 0;

  for (uint16_t x = ptr; x != 0; x >>= 1) i++;
  PutBits (es, es->p_len[i], es->p_code[i]);
  if (i > 1) PutBits (es, i - 1, ptr & (((1 << 16) - 1) >> (17 - i)));
}
#else
;
#endif

//static
 void SendBlock (struct LZHEncoderState_t *es)
#if 0
{
  uint16_t root, size, pos;

  root = MakeTree (es, NC, (int16_t *) (es->c_freq), es->c_len, (int16_t *) (es->c_code));
  size = es->c_freq[root];
  PutBits (es, 16, size);
  if (root >= NC)
  {
    CountFreq (es);
    root = MakeTree (es, NT, (int16_t *) (es->t_freq), es->p_len, (int16_t *) (es->p_code));
    if (root >= NT)
      WritePtrLen (es, NT, TBIT, 3);
    else
    {
      PutBits (es, TBIT, 0);
      PutBits (es, TBIT, root);
    }
    WriteCharLen (es);
  }
  else
  {
    PutBits (es, TBIT, 0);
    PutBits (es, TBIT, 0);
    PutBits (es, CBIT, 0);
    PutBits (es, CBIT, root);
  }
  root = MakeTree (es, NP, (int16_t *) (es->p_freq), es->p_len, (int16_t *) (es->p_code));
  if (root >= NP)
    WritePtrLen (es, NP, PBIT, -1);
  else
  {
    PutBits (es, PBIT, 0);
    PutBits (es, PBIT, root);
  }
  pos = 0;
  for (uint16_t i = 0; i < size; i++)
  {
    uint16_t flags = 0;

    if ((i & 7) == 0)
      flags = es->buf[pos++];
    else
      flags <<= 1;
    if ((flags & (1 << 7)) != 0)
    {
      uint16_t j = es->buf[pos++] + (1 << 8);
      PutBits (es, es->c_len[j], es->c_code[j]);
      j = (es->buf[pos] << 8) + es->buf[pos + 1];
      pos += 2;
      EncodePtr (es, j);
    }
    else
    {
      uint16_t j = es->buf[pos++];
      PutBits (es, es->c_len[j], es->c_code[j]);
    }
  }
  for (unsigned i = 0; i < NC; es->c_freq[i++] = 0);
  for (unsigned i = 0; i < NP; es->p_freq[i++] = 0);
}
#else
;
#endif

//static
 void Output (struct LZHEncoderState_t *es, uint16_t code, uint16_t c_ptr)
#if 0
{
  es->out_mask >>= 1;
  if (es->out_mask == 0)
  {
    es->out_mask = 1 << 7;
    if (es->out_pos >= es->WIN_SIZE - 24)
    {
      SendBlock (es);
      es->out_pos = 0;
    }
    es->c_pos = es->out_pos++;
    es->buf[es->c_pos] = 0;
  }
  es->buf[es->out_pos++] = code;
  es->c_freq[code]++;
  if (code >= 1 << 8)
  {
    es->buf[es->c_pos] |= es->out_mask;
    es->buf[es->out_pos++] = c_ptr >> 8;
    es->buf[es->out_pos++] = c_ptr;
    for (code = 0; c_ptr != 0; c_ptr >>= 1) code++;
    es->p_freq[code]++;
  }
}
#else
;
#endif

//static
 void InitSlide (struct LZHEncoderState_t *es)
#if 0
{
  memset (&es->level[es->DIC_SIZE], 1, 256 * sizeof (es->level[0])); // byte!
  memset (&es->pos_ptr[es->DIC_SIZE], 0, 256 * sizeof (es->pos_ptr[0]));
  memset (&es->parent[es->DIC_SIZE], 0, es->DIC_SIZE * sizeof (es->parent[0]));
  es->avail = 1;
  for (uint16_t i = 1; i < es->DIC_SIZE - 1; i++) es->next[i] = i + 1;
  es->next[es->DIC_SIZE - 1] = 0;
  memset (&es->next[2 * es->DIC_SIZE], 0, (MAX_HASH_VAL + 1 - 2 * es->DIC_SIZE) * sizeof (es->next[0]));
}
#else
;
#endif

//static
 int16_t Child (struct LZHEncoderState_t *es, int16_t pnode, uint8_t chr)
#if 0
{
  int16_t node = es->next[pnode + (chr << (DIC_BIT - 9)) + 2 * es->DIC_SIZE];
  for (es->parent[0] = pnode; es->parent[node] != pnode; node = es->next[node]);
  return node;
}
#else
;
#endif

//static
 void MakeChild (struct LZHEncoderState_t *es,
                       int16_t p_node,
                       uint8_t chr,
                       int16_t c_node)
#if 0
{
  int16_t i, j;

  i = p_node + (chr << (DIC_BIT - 9)) + 2 * es->DIC_SIZE;
  j = es->next[i];
  es->next[i] = c_node;
  es->next[c_node] = j;
  es->previous[j] = c_node;
  es->previous[c_node] = i;
  es->parent[c_node] = p_node;
  es->child_count[p_node]++;
}
#else
;
#endif

//static
 void SplitTree (struct LZHEncoderState_t *es, int16_t old)
#if 0
{
  int16_t new, i;

  new = es->avail;
  es->avail = es->next[new];
  es->child_count[new] = 0;
  i = es->previous[old];
  es->previous[new] = i;
  es->next[i] = new;
  i = es->next[old];
  es->next[new] = i;
  es->previous[i] = new;
  es->parent[new] = es->parent[old];
  es->level[new] = es->match_len;
  es->pos_ptr[new] = es->pos;
  MakeChild (es, new, es->stream[es->match_pos + es->match_len], old);
  MakeChild (es, new, es->stream[es->pos + es->match_len], es->pos);
}
#else
;
#endif

//static
 void InsertNode (struct LZHEncoderState_t *es)
#if 0
{
  int16_t i, j;

  if (es->match_len >= 4)
  {
    int16_t k;

    es->match_len--;
    j = (es->match_pos + 1) | es->DIC_SIZE;
    for (i = es->parent[j]; i == 0; i = es->parent[j])
      j = es->next[j];
    for (; es->level[i] >= es->match_len; i = es->parent[i])
      j = i;
    for (k = i; es->pos_ptr[k] < 0; k = es->parent[k])
      es->pos_ptr[k] = es->pos;
    if (k < es->DIC_SIZE)
      es->pos_ptr[k] = es->pos | PERC_FLAG;
  }
  else
  {
    uint8_t chr;

    i = es->stream[es->pos] + es->DIC_SIZE;
    chr = es->stream[es->pos + 1];
    j = Child (es, i, chr);
    if (j == 0)
    {
      MakeChild (es, i, chr, es->pos);
      es->match_len = 1;
      return; //InsertNode
    }
    es->match_len = 2;
  }

  for (;; es->match_len++)
  {
    int16_t len;
    const uint8_t *ptr1, *ptr2;

    if (j >= es->DIC_SIZE)
    {
      len = MAX_MATCH;
      es->match_pos = j;
    }
    else
    {
      len = es->level[j];
      es->match_pos = es->pos_ptr[j] & ~(1 << 15);
    }
    if (es->match_pos >= es->pos)
      es->match_pos -= es->DIC_SIZE;
    ptr1 = &es->stream[es->pos + es->match_len];
    ptr2 = &es->stream[es->match_pos + es->match_len];
    for (; es->match_len < len; es->match_len++)
    {
      if (*ptr1 != *ptr2)
      {
        SplitTree (es, j);
        return; //InsertNode
      }
      ptr1++;
      ptr2++;
    }
    if (es->match_len >= MAX_MATCH) break;
    es->pos_ptr[j] = es->pos;
    i = j;
    j = Child (es, i, *ptr1);
    if (j == 0)
    {
      MakeChild (es, i, *ptr1, es->pos);
      return; //InsertNode
    }
  }

  {
    int16_t t = es->previous[j];
    es->previous[es->pos] = t;
    es->next[t] = es->pos;
  }
  {
    int16_t t = es->next[j];
    es->next[es->pos] = t;
    es->previous[t] = es->pos;
  }
  es->parent[es->pos] = i;
  es->parent[j] = 0;
  es->next[j] = es->pos;

  //EXIT //InsertNode
}
#else
;
#endif

//static
 void DeleteNode (struct LZHEncoderState_t *es)
#if 0
{
  int16_t i, j, k, l;
  int16_t perc_idx;

  if (es->parent[es->pos] == 0) return; //DeleteNode
  i = es->previous[es->pos];
  j = es->next[es->pos];
  es->next[i] = j;
  es->previous[j] = i;
  i = es->parent[es->pos];
  es->parent[es->pos] = 0;

  es->child_count[i]--;
  if ((i >= es->DIC_SIZE) || (es->child_count[i] > 1)) return; //DeleteNode
  k = es->pos_ptr[i] & ~PERC_FLAG;
  if (k >= es->pos) k -= es->DIC_SIZE;

  j = k;
  perc_idx = es->parent[i];
  for (l = es->pos_ptr[perc_idx]; (l & PERC_FLAG) != 0; l = es->pos_ptr[perc_idx])
  {
    l &= ~PERC_FLAG;
    if (l >= es->pos) l -= es->DIC_SIZE;
    if (l > j) j = l;
    es->pos_ptr[perc_idx] = j | es->DIC_SIZE;
    perc_idx = es->parent[perc_idx];
  }
  if (perc_idx < es->DIC_SIZE)
  {
    if (l >= es->pos) l -= es->DIC_SIZE;
    if (l > j) j = l;
    es->pos_ptr[perc_idx] = j | es->DIC_SIZE | PERC_FLAG;
  }

  j = Child (es, i, es->stream[k + es->level[i]]);
  {
    int16_t t = es->previous[j];
    int16_t u = es->next[j];
    es->next[t] = u;
    es->previous[u] = t;
  }
  {
    int16_t t = es->previous[i];
    es->next[t] = j;
    es->previous[j] = t;
  }
  {
    int16_t t = es->next[i];
    es->previous[t] = j;
    es->next[j] = t;
  }
  es->parent[j] = es->parent[i];
  es->parent[i] = 0;
  es->next[i] = es->avail;
  es->avail = i;

  //EXIT //DeleteNode
}
#else
;
#endif

//static
 void GetNextMatch (struct LZHEncoderState_t *es)
#if 0
{
  es->remain--;
  if (++es->pos == 2 * es->DIC_SIZE)
  {
    int16_t bits;

    memmove (es->stream, &es->stream[es->DIC_SIZE], (es->DIC_SIZE + MAX_MATCH) * sizeof (es->stream[0]));
    bits = ReadDataBlock (es, &es->stream[es->DIC_SIZE + MAX_MATCH], es->DIC_SIZE);
    es->remain += bits;
    es->pos = es->DIC_SIZE;
  }
  DeleteNode (es);
  InsertNode (es);
}
#else
;
#endif

// ultra: compression type flag (false=default, true=ultra)
//static
 uint32_t LZH_do_compress (const void *source, void *dest, uint32_t size, bool ultra)
#if 1
{
  uint32_t result = 0;
  struct LZHEncoderState_t *es;
  int16_t last_match_len, last_match_pos;

  if ((es = malloc (sizeof (struct LZHEncoderState_t))) == NULL) goto _exit;
  if (ultra)
  {
    es->WIN_SIZE = WIN_SIZE_MAX;
    es->DIC_SIZE = DIC_SIZE_MAX;
  }
  else
  {
    es->WIN_SIZE = WIN_SIZE_DEF;
    es->DIC_SIZE = DIC_SIZE_DEF;
  }
  //es->l_tree
  //es->r_tree
  //es->p_len
  //es->c_len
  //es->heap
  //es->len_count
  //es->c_freq
  //es->p_freq
  //es->t_freq
  //es->c_code
  //es->p_code
  //es->freq
  //es->sort_ptr
  es->pos_ptr = NULL;
  es->buf = NULL;
  //es->len
  es->stream = NULL;
  es->child_count = NULL;
  es->level = NULL;
  es->parent = NULL;
  es->previous = NULL;
  es->next = NULL;
  //es->bits
  //es->heap_size
  //es->remain
  //es->match_len
  //es->sbit_buf
  //es->bit_count
  //es->depth
  //es->c_pos
  //es->pos
  //es->out_pos
  //es->match_pos
  //es->out_mask
  //es->avail
  es->input_buffer = source;
  es->output_buffer = dest;
  es->input_buffer_idx = 0;
  es->output_buffer_idx = 0;
  es->input_buffer_size = size;

  es->output_buffer[es->output_buffer_idx++] = ultra ? 1 : 0; // put compression type flag
  memmove (&es->output_buffer[es->output_buffer_idx], &size, sizeof (size));
  es->output_buffer_idx += sizeof (size);

  progress_old_value = UINT8_NULL;
  progress_value = size;

  if ((es->stream = malloc ((2 * es->DIC_SIZE + MAX_MATCH) * sizeof (es->stream[0]))) == NULL) goto _exit;
  if ((es->level = malloc ((es->DIC_SIZE + 256) * sizeof (es->level[0]))) == NULL) goto _exit;
  if ((es->child_count = malloc ((es->DIC_SIZE + 256) * sizeof (es->child_count[0]))) == NULL) goto _exit;
  if ((es->pos_ptr = malloc ((es->DIC_SIZE + 256) * sizeof (es->pos_ptr[0]))) == NULL) goto _exit;
  if ((es->parent = malloc (es->DIC_SIZE * 2 * sizeof (es->parent[0]))) == NULL) goto _exit;
  if ((es->previous = malloc (es->DIC_SIZE * 2 * sizeof (es->previous[0]))) == NULL) goto _exit;
  if ((es->next = malloc ((MAX_HASH_VAL + 1) * sizeof (es->next[0]))) == NULL) goto _exit;

  es->depth = 0;
  InitSlide (es);
  if ((es->buf = malloc (es->WIN_SIZE * sizeof (es->buf[0]))) == NULL) goto _exit;
  es->buf[0] = 0;
  memset (es->c_freq, 0, sizeof (es->c_freq));
  memset (es->p_freq, 0, sizeof (es->p_freq));
  es->out_pos = 0;
  es->out_mask = 0;
  es->bit_count = 8;
  es->sbit_buf = 0;
  es->remain = ReadDataBlock (es, &es->stream[es->DIC_SIZE], es->DIC_SIZE + MAX_MATCH);
  es->match_len = 0;
  es->pos = es->DIC_SIZE;
  InsertNode (es);
  if (es->match_len > es->remain)
    es->match_len = es->remain;

  while (es->remain > 0)
  {
    last_match_len = es->match_len;
    last_match_pos = es->match_pos;
    GetNextMatch (es);
    if (es->match_len > es->remain)
      es->match_len = es->remain;
    if ((es->match_len > last_match_len) || (last_match_len < THRESHOLD))
      Output (es, es->stream[es->pos - 1], 0);
    else
    {
      Output (es, last_match_len + (256 - THRESHOLD), (es->pos - last_match_pos - 2) & (es->DIC_SIZE - 1));
      for (last_match_len--; last_match_len > 0; last_match_len--)
        GetNextMatch (es);
      if (es->match_len > es->remain)
        es->match_len = es->remain;
    }
  }

  SendBlock (es);
  PutBits (es, 7, 0);
  result = es->output_buffer_idx;

_exit:
  if (es != NULL)
  {
    if (es->pos_ptr != NULL) free (es->pos_ptr);
    if (es->buf != NULL) free (es->buf);
    if (es->stream != NULL) free (es->stream);
    if (es->child_count != NULL) free (es->child_count);
    if (es->level != NULL) free (es->level);
    if (es->parent != NULL) free (es->parent);
    if (es->previous != NULL) free (es->previous);
    if (es->next != NULL) free (es->next);
    free (es);
  }
  return result;
}
#else
;
#endif

uint32_t LZH_compress (const void *source, void *dest, uint32_t size)
#if 1
{
  return LZH_do_compress (source, dest, size, 0);
}
#else
;
#endif

uint32_t LZH_compress_ultra (const void *source, void *dest, uint32_t size)
#if 1
{
  return LZH_do_compress (source, dest, size, 1);
}
#else
;
#endif
