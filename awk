BEGIN {
 PREPEND="";
}
{
 ORS=" ";
 print PREPEND "'" $1 "' every ::b::a title '" $1 "' w lines";
 PREPEND=",";
}
