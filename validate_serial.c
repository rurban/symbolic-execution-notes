#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char *progname)
{
  printf("Usage: %s <serial number>\n", progname);
  printf("Serial Number is the in the form XXXXX-XXXXX\n");
  exit(0);
}

int is_digit(int d)
{
  if(d >= 48 && d <= 57)
    return 1;

  return 0;
}

int is_char(int d)
{
  if(d >= 65 && d <= 90)
    return 1;

  return 0;
}

int validate_sn(char *sn)
{
  // constraint 0: the whole SN has 11 chars
  if (strlen(sn) != 11)
    return 0;

  // constraint 1: index below must be equal to '-'
  if(sn[5] != 45)
    return 0;

  // constraint 2: the first char of first XXXXX sequence must be 1 and first char of second XXXXX sequence must be 5 
  if(sn[0] != 49)
    return 0;
  if(sn[6] != 53)
    return 0;

  // constraint 3: 2nd char is a digit, 7th char is digit
  if(is_digit(sn[1]) != 1)
    return 0;
  if(is_digit(sn[7]) != 1)
    return 0;

  // constraint 4: 3rd and 8th chars are uper case letters
  if(is_char(sn[2]) != 1)
    return 0; 
  if(is_char(sn[8]) != 1)
    return 0;

  // constraint 5: last 2 chars of every XXXXX are numbers
  if(is_digit(sn[3]) != 1)
    return 0;
  if(is_digit(sn[4]) != 1)
    return 0;
  if(is_digit(sn[9]) != 1)
    return 0;
  if(is_digit(sn[10]) != 1)
    return 0;

  // constraint 6: the last 2 chars of every XXXXX sequence must be the same
  if(sn[3] != sn[4])
    return 0;
  if(sn[9] != sn[10])
    return 0;

  return 1;
}

int main(int argc, char *argv[])
{
  char *serial = argv[1];

  if (argc < 2)
    usage(argv[0]);

  if (validate_sn(serial))
    printf("Thank You. Your license is now active.\n");
  else
    printf("Invalid Serial Number. Please try again.\n");

  return 0;
}
