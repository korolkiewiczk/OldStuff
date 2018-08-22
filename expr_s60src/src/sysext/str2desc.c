void str2desc(char *str, TDes& desc)
{
 TPtrC8 ptr(reinterpret_cast<const TUint8*>(str));
 desc.Copy(ptr);
}
