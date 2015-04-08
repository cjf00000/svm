#ifndef __CONSTS
#define __CONSTS

#include <cstdio>
#include <glog/logging.h>
#include <gflags/gflags.h>

#define infty 1e100

#include <random>

extern std::default_random_engine generator;

// Google Log hack
#define LI LOG(INFO)
#define LW LOG(WARNING)
#define LD DLOG(INFO)

// Google flags hack
static void print_help() {
  fprintf(stderr, "Program Flags:\n");
  std::vector<google::CommandLineFlagInfo> all_flags;
  google::GetAllFlags(&all_flags);
  for (const auto& flag : all_flags) {
    if (flag.filename.find("src/") != 0) // HACK: filter out built-in flags
      fprintf(stderr,
              "-%s: %s (%s, default:%s)\n",
              flag.name.c_str(),
              flag.description.c_str(),
              flag.type.c_str(),
              flag.default_value.c_str());
  }
  exit(1);
}

// Google flags hack
static void print_flags() {
  LI << "---------------------------------------------------------------------";
  std::vector<google::CommandLineFlagInfo> all_flags;
  google::GetAllFlags(&all_flags);
  for (const auto& flag : all_flags) {
    if (flag.filename.find("src/") != 0) // HACK: filter out built-in flags
      LI << flag.name << ": " << flag.current_value;
  }
  LI << "---------------------------------------------------------------------";
}

#endif 
