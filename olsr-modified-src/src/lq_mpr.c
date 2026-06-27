/*
 * The olsr.org Optimized Link-State Routing daemon (olsrd)
 *
 * (c) by the OLSR project
 *
 * See our Git repository to find out who worked on this file
 * and thus is a copyright holder on it.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 */

#include "defs.h"
#include "neighbor_table.h"
#include "two_hop_neighbor_table.h"
#include "link_set.h"
#include "lq_mpr.h"
#include "scheduler.h"
#include "lq_plugin.h"
#include "olsr.h"

struct neighbor_2_entry neighbor_2hop[8];
union olsr_ip_addr neighbor_addr[7];

bool init = false;

struct link
{
  int z;
  struct neighbor_2_entry *neighbor_2hop;
  struct link *next;
  struct link *prev;
};

struct olsr_addr_set
{
  union olsr_ip_addr neighbor_main_addr;
  struct olsr_addr_set *next;
  struct olsr_addr_set *prev;
};

// void test()
// {
//   struct neighbor_entry* neighbor[7];
//   struct neighbor_list_entry* neibor_list[8];
//   int i;
//   for(i = 0;i<7;i++)
//   {
//     neighbor_addr[i].v4.s_addr = 0x01010101+i;
//     neighbor[i] = olsr_insert_test_neighbor_table(neighbor_addr+i);
//   }

//   neibor_list[0] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[0]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[0]->next->neighbor = neighbor[0];
//   neibor_list[0]->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[0]->next->next->neighbor = neighbor[1];
//   neibor_list[0]->next->next->next = &neighbor_2hop[0].neighbor_2_nblist;

//   neibor_list[1] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[1]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[1]->next->neighbor = neighbor[0];
//   neibor_list[1]->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[1]->next->next->neighbor = neighbor[1];
//   neibor_list[1]->next->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[1]->next->next->next->neighbor = neighbor[2];
//   neibor_list[1]->next->next->next->next = &neighbor_2hop[1].neighbor_2_nblist;

//   neibor_list[2] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[2]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[2]->next->neighbor = neighbor[1];
//   neibor_list[2]->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[2]->next->next->neighbor = neighbor[2];
//   neibor_list[2]->next->next->next = &neighbor_2hop[2].neighbor_2_nblist;

//   neibor_list[3] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[3]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[3]->next->neighbor = neighbor[1];
//   neibor_list[3]->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[3]->next->next->neighbor = neighbor[2];
//   neibor_list[3]->next->next->next = &neighbor_2hop[3].neighbor_2_nblist;

//   neibor_list[4] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[4]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[4]->next->neighbor = neighbor[2];
//   neibor_list[4]->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[4]->next->next->neighbor = neighbor[3];
//   neibor_list[4]->next->next->next = &neighbor_2hop[4].neighbor_2_nblist;

//   neibor_list[5] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[5]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[5]->next->neighbor = neighbor[2];
//   neibor_list[5]->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[5]->next->next->neighbor = neighbor[3];
//   neibor_list[5]->next->next->next = &neighbor_2hop[5].neighbor_2_nblist;

//   neibor_list[6] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[6]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[6]->next->neighbor = neighbor[3];
//   neibor_list[6]->next->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[6]->next->next->neighbor = neighbor[4];
//   neibor_list[6]->next->next->next = &neighbor_2hop[6].neighbor_2_nblist;

//   neibor_list[7] = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[7]->next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
//   neibor_list[7]->next->neighbor = neighbor[6];
//   neibor_list[7]->next->next = &neighbor_2hop[7].neighbor_2_nblist;

//   for(i = 0 ;i< 8 ;i++)
//   {
//     neighbor_2hop[i].neighbor_2_addr.v4.s_addr = 0x02020201+i;
//     neighbor_2hop[i].neighbor_2_nblist = *neibor_list[i];

//     olsr_insert_two_hop_neighbor_table(neighbor_2hop+i);
//   }

// }

void
olsr_calculate_lq_mpr(void)
{
  struct ipaddr_str buf;
  struct neighbor_2_entry *neigh2;
  struct neighbor_list_entry *walker;
  struct neighbor_list_entry *Ls;
  struct neighbor_list_entry *Lstemp;
  int i,k;
  struct neighbor_entry *neigh;
  olsr_linkcost best, best_1hop;
  struct olsr_addr_set *mpr_addr;
  struct olsr_addr_set *mpr_temp;
  struct olsr_addr_set *NN;
  struct olsr_addr_set *nntemp;
  struct olsr_addr_set *NN_replace;
  struct olsr_addr_set *deleted_addrs;
  struct olsr_addr_set *deleted_temp;
  struct neighbor_entry *N;
  struct neighbor_entry *temp;
  struct neighbor_2_entry *N2;
  struct neighbor_2_entry *n2temp;

  struct link *link_n;
  struct link *link_temp;
  
  // if(!init)
  // {
  //   test();
  //   init = true;
  // }

  bool mpr_changes = false;
  bool ok = false;
  N = olsr_malloc(sizeof(struct neighbor_entry),"n");
  temp = N;

  k = 0;

  //printf("enter!\n");
  OLSR_FOR_ALL_NBR_ENTRIES(neigh) {
    /* Memorize previous MPR status. */

    neigh->was_mpr = neigh->is_mpr;

    /* Clear current MPR status. */

    neigh->is_mpr = false;

    /* In this pass we are only interested in WILL_ALWAYS neighbours */

    
    if (neigh->status == SYM) {
      memcpy(temp,neigh,sizeof(struct neighbor_entry));
      temp->next = olsr_malloc(sizeof(struct neighbor_entry),"n");
      temp->next->prev = temp;
      temp = temp ->next;
      ok = true;
      k++;
    }
  }
  OLSR_FOR_ALL_NBR_ENTRIES_END(neigh);
  if(ok)
  {
    temp->prev->next = NULL;
    free(temp);
  }
  else
  {
    free(N);
    N = NULL;
  }

  ok = false;

  N2 = olsr_malloc(sizeof(struct neighbor_2_entry),"n2");
  n2temp = N2;
  //printf("enter2!\n");
  for (i = 0; i < HASHSIZE; i++) {
    /* loop through all 2-hop neighbours */
     int cnt = 0;
     bool ok_temp = false;
    for (neigh2 = two_hop_neighbortable[i].next; neigh2 != &two_hop_neighbortable[i]; neigh2 = neigh2->next) {
      cnt++;
      Ls = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
      Ls->neighbor = NULL;
      Lstemp = Ls;
      for (walker = neigh2->neighbor_2_nblist.next; walker != &neigh2->neighbor_2_nblist; walker = walker->next)
      {
        temp = olsr_lookup_neighbor_table(&walker->neighbor->neighbor_main_addr);
        //printf("temp is :%s\n",olsr_ip_to_string(&buf,&walker->neighbor->neighbor_main_addr));
        if(temp == NULL)
          continue;
        else
        {
          if(temp->status!= SYM)
            continue;
          if(temp->status == WILL_NEVER)
            continue;
          temp = olsr_lookup_neighbor_table(&neigh2->neighbor_2_addr);
          if(temp)
            continue;
        }
        memcpy(Lstemp,walker,sizeof(struct neighbor_list_entry));
        Lstemp -> next = olsr_malloc(sizeof(struct neighbor_list_entry),"ls");
        Lstemp->next->prev = Lstemp;
        Lstemp = Lstemp->next;
        ok_temp = true;
      }
      if(ok_temp)
      {
        Lstemp->prev->next = NULL;
        free(Lstemp);
        memcpy(n2temp,neigh2,sizeof(struct neighbor_2_entry));
        n2temp->neighbor_2_nblist = *Ls;
        n2temp -> next = olsr_malloc(sizeof(struct neighbor_2_entry),"n2");
        n2temp->next->prev = n2temp;
        n2temp = n2temp->next;
        ok = true;
        ok_temp = false;
      }
      else
      {
        free(Ls);
        Ls = NULL;
      }
    }
  }
  if(ok)
  {
    n2temp->prev->next = NULL;
    free(n2temp);
  }
  else
  {
    free(N2);
    N2 = NULL;
  }
  ok = false;
  mpr_addr = olsr_malloc(sizeof(struct olsr_addr_set),"mpr");
  mpr_temp = mpr_addr;
  for(temp = N;temp!=NULL;temp=temp->next)
  {
    if(temp->willingness ==WILL_ALWAYS)
    {
      mpr_temp->neighbor_main_addr = temp->neighbor_main_addr;
      mpr_temp -> next = olsr_malloc(sizeof(struct olsr_addr_set),"n2");
      mpr_temp->next->prev = mpr_temp;
      mpr_temp = mpr_temp->next;
      ok = true;
    }
  }

  ok = false;

  NN = olsr_malloc(sizeof(struct olsr_addr_set),"nn");
  nntemp = NN;
  u_int32_t* order = olsr_malloc(sizeof(uint8_t)*k,"order");
  int max = 0;
  k = 0;
  for(temp = N;temp!=NULL;temp=temp->next)
  {
    int r= 0;
    for(n2temp = N2;n2temp!=NULL;n2temp = n2temp->next)
    {
      struct neighbor_list_entry * ltemp;
      ltemp = &n2temp->neighbor_2_nblist;
      while(ltemp)
      {
        if(ipequal(&temp->neighbor_main_addr , &ltemp->neighbor->neighbor_main_addr))
        {
          r++;
        }
        ltemp = ltemp->next;
      }
    }
    if(r>0)
    {
      if(max<=r)
        max = r;
      *(order+k) = r; k++; 
      nntemp->neighbor_main_addr = temp->neighbor_main_addr;
      nntemp -> next = olsr_malloc(sizeof(struct olsr_addr_set),"nn");
      nntemp->next->prev = nntemp;
      nntemp = nntemp->next;
      ok = true;
    }
  }
  if(ok)
  {
    nntemp->prev->next = NULL;
    free(nntemp);
  }
  else
  {
    free(NN);
    NN = NULL;
  }

  nntemp = NN;

  for(i = 0;i< k ;i++)
  {
    nntemp = nntemp->next;
  }

  ok = false;
  link_n = olsr_malloc(sizeof(struct link),"link_n");
  link_temp = link_n;
  for(n2temp = N2;n2temp!=NULL;n2temp = n2temp->next)
  {
    struct neighbor_list_entry * ltemp;
    int z = 0;
    ltemp = &n2temp->neighbor_2_nblist;
    while(ltemp)
    {
      for(temp = N;temp!=NULL;temp=temp->next)
      {            
        if(ipequal(&temp->neighbor_main_addr , &ltemp->neighbor->neighbor_main_addr))
        {
          z++;
        }
      }
      ltemp = ltemp->next;
    }
    link_temp->z = z;
    link_temp->neighbor_2hop = n2temp;
    link_temp -> next = olsr_malloc(sizeof(struct link),"link_n");
    link_temp->next->prev = link_temp;
    link_temp = link_temp->next;
    ok = true;
  }
  if(ok)
  {
    link_temp->prev->next = NULL;
    free(link_temp);
  }
  else
  {
    free(link_n);
    link_n = NULL;
  }

  ok = false;
  for(i = 1;i <= max; i++)
  {
    nntemp = NN;
    int j;
    for(j = 0 ;j < k ;j++)
    {
      if(order[j] == i)
      {
        struct olsr_addr_set* delet_nntemp;
        struct olsr_addr_set* dl;

        deleted_addrs = olsr_malloc(sizeof(struct olsr_addr_set),"deleted_addrs"); 
        deleted_temp = deleted_addrs;
        
        order[j] = 0;
        for(link_temp = link_n ; link_temp!=NULL;link_temp = link_temp->next)
        {
          struct neighbor_list_entry * ltemp;
          ltemp = &link_temp->neighbor_2hop->neighbor_2_nblist;
          while(ltemp)
          {
            // printf("z is %d,addr is :%s\n",link_temp->z,olsr_ip_to_string(&buf,&ltemp->neighbor->neighbor_main_addr));
            if(ipequal(&nntemp->neighbor_main_addr , &ltemp->neighbor->neighbor_main_addr))
            {
              (link_temp->z)--;
              deleted_temp->neighbor_main_addr = link_temp->neighbor_2hop->neighbor_2_addr;
              deleted_temp -> next = olsr_malloc(sizeof(struct olsr_addr_set),"deleted_addrs");
              deleted_temp->next->prev = deleted_temp;
              deleted_temp = deleted_temp->next;
              ok =true;
            }
            ltemp = ltemp->next;
          }
         // printf("z is:%d,neighbor2hop is :%s\n",link_temp->z,olsr_ip_to_string(&buf,&link_temp->neighbor_2hop->neighbor_2_addr));
        }
        if(ok)
        {
          deleted_temp->prev->next = NULL;
          free(deleted_temp);
          deleted_temp = NULL;
          ok = false;
        }
        else
        {
          free(deleted_temp);
          deleted_addrs = NULL;
        }
        int flag = 0;
        for(link_temp = link_n;link_temp!=NULL;link_temp = link_temp->next)
        {
          if(link_temp->z<=0)
          {
            //printf("flag =1\n");
            flag = 1;
            break;
          }
        }
        if(flag == 0)
        {
          if(nntemp == NN)
          {
            NN = NN->next;
          }
          else if(nntemp ->next ==NULL)
          {
            nntemp->prev->next =  NULL;
          }
          else{
            nntemp ->prev->next = nntemp->next;
            nntemp ->next->prev = nntemp->prev;
          }
        }
        else{
            mpr_temp ->neighbor_main_addr = nntemp ->neighbor_main_addr;
            //printf("mprtemp is :%s\n",olsr_ip_to_string(&buf,&mpr_temp ->neighbor_main_addr));
            mpr_temp -> next = olsr_malloc(sizeof(struct olsr_addr_set),"n2");
            mpr_temp->next->prev = mpr_temp;
            mpr_temp = mpr_temp->next;
            //printf("%p\n",mpr_temp->prev);
            ok = true;
            //memset(mpr_temp,0,sizeof(struct olsr_addr_set));
            //printf("%p\n",mpr_temp->prev);
            for(dl = deleted_addrs; dl != NULL;dl = dl->next)
            {
              for(link_temp = link_n;link_temp!=NULL;link_temp = link_temp->next)
              {
                if(ipequal(&link_temp->neighbor_2hop->neighbor_2_addr , &dl->neighbor_main_addr))
                  (link_temp->z)++;
              }
            }
          if(nntemp == NN)
          {
            NN = NN->next;
          }
          else if(nntemp ->next ==NULL)
          {
            nntemp->prev->next =  NULL;
          }
          else{
            nntemp ->prev->next = nntemp->next;
            nntemp ->next->prev = nntemp->prev;
          }
           // printf("dierbu\n");
            for(link_temp = link_n ; link_temp!=NULL;link_temp = link_temp->next)
            {
              struct neighbor_list_entry * ltemp;
              ltemp = &link_temp->neighbor_2hop->neighbor_2_nblist;
              while(ltemp)
              {
                if(ipequal(&nntemp->neighbor_main_addr , &ltemp->neighbor->neighbor_main_addr))
                {
                  if(link_temp == link_n)
                  {
                    link_n = link_n->next;
                  }
                  else if(link_temp ->next ==NULL)
                  {
                    link_temp->prev->next =  NULL;
                  }
                  else{
                    link_temp ->prev->next = link_temp->next;
                    link_temp ->next->prev = link_temp->prev;
                  }
             //     printf("disanbu\n");
                    free(link_temp);
                    break;
                }
                ltemp = ltemp->next;
              }
            }
        }
        delet_nntemp = nntemp;
        nntemp = nntemp->next;
        free(delet_nntemp);

        dl = deleted_addrs;
        while(dl)
        {
          struct olsr_addr_set* x;
          x = dl;
          dl =  dl->next;
          free(x);
        }

        // for(link_temp = link_n ; link_temp!=NULL;link_temp = link_temp->next)
        // {
        //   printf("link_n is %d\n",link_temp->z);
        // }
      }
      if(order[j]!=0)
        nntemp = nntemp->next;
    }
  }
  //printf("disibu\n");
  if(ok)
  {
    mpr_temp->prev->next = NULL;
    free(mpr_temp);
    mpr_temp = NULL;
    ok = false;
  }
  else
  {
    free(mpr_addr);
    mpr_addr = NULL;
  }
  OLSR_FOR_ALL_NBR_ENTRIES(neigh) {
    for(mpr_temp = mpr_addr;mpr_temp!=NULL;mpr_temp=mpr_temp->next)
    {
      if(ipequal(&neigh->neighbor_main_addr,&mpr_temp->neighbor_main_addr))
      {
        printf("mpr is :%s\n",olsr_ip_to_string(&buf,&mpr_temp ->neighbor_main_addr));
          neigh->is_mpr = true;
          if(neigh->is_mpr!=neigh->was_mpr)
            mpr_changes = true;
      }
    }
  }
  OLSR_FOR_ALL_NBR_ENTRIES_END(neigh);

  temp = N;
  while(temp)
  {
    struct neighbor_entry* x;
    x = temp;
    temp = temp->next;
    free(x);
  }

  n2temp = N2; 
  while(n2temp)
  {
    Lstemp = n2temp->neighbor_2_nblist.next;
    while(Lstemp)
    {
      struct neighbor_list_entry* x;
      x = Lstemp;
      Lstemp = Lstemp->next;
      free(x);
    }
    n2temp = n2temp->next;
  }

  n2temp = N2;
  while(n2temp)
  {
    struct neighbor_2_entry* x;
    x = n2temp;
    n2temp = n2temp->next;
    free(x);
  }

  nntemp = NN;
  while(nntemp)
  {
    struct olsr_addr_set* x;
    x = nntemp;
    nntemp = nntemp->next;
    free(x);
  }

  link_temp = link_n;
  while(link_temp)
  {
    struct link* x;
    x = link_temp;
    link_temp = link_temp->next;
    free(x);
  } 

  mpr_temp = mpr_addr;
  while(mpr_temp)
  {
    struct olsr_addr_set* x;
    x = mpr_temp;
    mpr_temp = mpr_temp->next;
    free(x);
  }     

  if (mpr_changes && olsr_cnf->tc_redundancy > 0)
    signal_link_changes(true);
}


// void
// olsr_calculate_lq_mpr(void)
// {
//   struct neighbor_2_entry *neigh2;
//   struct neighbor_list_entry *walker;
//   int i, k;
//   struct neighbor_entry *neigh;
//   olsr_linkcost best, best_1hop;
//   bool mpr_changes = false;

//   OLSR_FOR_ALL_NBR_ENTRIES(neigh) {

//     /* Memorize previous MPR status. */

//     neigh->was_mpr = neigh->is_mpr;

//     /* Clear current MPR status. */

//     neigh->is_mpr = false;

//     /* In this pass we are only interested in WILL_ALWAYS neighbours */

//     if (neigh->status == NOT_SYM || neigh->willingness != WILL_ALWAYS) {
//       continue;
//     }

//     neigh->is_mpr = true;

//     if (neigh->is_mpr != neigh->was_mpr) {
//       mpr_changes = true;
//     }

//   }
//   OLSR_FOR_ALL_NBR_ENTRIES_END(neigh);

//   for (i = 0; i < HASHSIZE; i++) {
//     /* loop through all 2-hop neighbours */

//     for (neigh2 = two_hop_neighbortable[i].next; neigh2 != &two_hop_neighbortable[i]; neigh2 = neigh2->next) {
//       best_1hop = LINK_COST_BROKEN;

//       /* check whether this 2-hop neighbour is also a neighbour */

//       neigh = olsr_lookup_neighbor_table(&neigh2->neighbor_2_addr);

//       /* if it's a neighbour and also symmetric, then examine
//          the link quality */

//       if (neigh != NULL && neigh->status == SYM) {
//         /* if the direct link is better than the best route via
//          * an MPR, then prefer the direct link and do not select
//          * an MPR for this 2-hop neighbour */

//         /* determine the link quality of the direct link */

//         struct link_entry *lnk = get_best_link_to_neighbor(&neigh->neighbor_main_addr);

//         if (!lnk)
//           continue;

//         best_1hop = lnk->linkcost;

//         /* see wether we find a better route via an MPR */

//         for (walker = neigh2->neighbor_2_nblist.next; walker != &neigh2->neighbor_2_nblist; walker = walker->next)
//           if (walker->path_linkcost < best_1hop)
//             break;

//         /* we've reached the end of the list, so we haven't found
//          * a better route via an MPR - so, skip MPR selection for
//          * this 1-hop neighbor */

//         if (walker == &neigh2->neighbor_2_nblist)
//           continue;
//       }

//       /* find the connecting 1-hop neighbours with the
//        * best total link qualities */

//       /* mark all 1-hop neighbours as not selected */

//       for (walker = neigh2->neighbor_2_nblist.next; walker != &neigh2->neighbor_2_nblist; walker = walker->next)
//         walker->neighbor->skip = false;

//       for (k = 0; k < olsr_cnf->mpr_coverage; k++) {
//         /* look for the best 1-hop neighbour that we haven't
//          * yet selected */

//         neigh = NULL;
//         best = LINK_COST_BROKEN;

//         for (walker = neigh2->neighbor_2_nblist.next; walker != &neigh2->neighbor_2_nblist; walker = walker->next)
//           if (walker->neighbor->status == SYM && !walker->neighbor->skip && walker->path_linkcost < best) {
//             neigh = walker->neighbor;
//             best = walker->path_linkcost;
//           }

//         /* Found a 1-hop neighbor that we haven't previously selected.
//          * Use it as MPR only when the 2-hop path through it is better than
//          * any existing 1-hop path. */
//         if ((neigh != NULL) && (best < best_1hop)) {
//           neigh->is_mpr = true;
//           neigh->skip = true;

//           if (neigh->is_mpr != neigh->was_mpr)
//             mpr_changes = true;
//         }

//         /* no neighbour found => the requested MPR coverage cannot
//          * be satisfied => stop */

//         else
//           break;
//       }
//     }
//   }

//   if (mpr_changes && olsr_cnf->tc_redundancy > 0)
//     signal_link_changes(true);
// }

/*
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */
