#ifndef __POLICY_H__
#define __POLICY_H__

#include "common.h"

// Disease control policy

// TODO: testing policy, quarantine
typedef struct policy_s {
  // Is social distancing recommended?
  bool dist_recommend;

  // Are stay-at-home orders active for people with symptoms?
  bool dist_home_symp;

  // Are stay-at-home orders active for everyone?
  bool dist_home_all;

  // Are field hospitals and improvised capacity expansion measures in place?
  bool temp_hospitals;

  // Is maximum temporary hospital capacity being expanded?
  bool temp_hospital_expansion;

} policy_t;

#endif
