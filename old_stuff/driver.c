#include <stdio.h>
#include "cdg.h"

static void k();
static void j();

void main() {
  k();
  return;
}

void k() {
  j();
}
void j() {}
