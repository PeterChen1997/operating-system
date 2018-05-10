int count = 1;
int value = 1;
int buf[10];
void main()
{
  asm(
      "cld nt"
      "rep nt"
      "stosl"
      :
      : "c"(count), "a"(value), "D"(buf[0])
      : "%ecx", "%edi");
}