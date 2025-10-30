//5 ZDK:

td struct _postfix* Position;
td struct _postfix{
  double number;
Position next;
}postfix;
1)int main(){
  postifx head = {number=0; next = NULL}
  return 0;
}
2)funkcija nekakvka za otvoriti dat procitat sta je zapisano
    -buffer
    -zatvorit datoteku(unutar funkcije nemoj salt pointer van zatvori zdk u funkciji)

3)
  a)gledamo jeli procitani znak broj
  b)jeli matematicki simbol
  c)trash podatak ni jedno ni drugo ili visak mat simbola il brojeva

push(&head, num)//ubacujemo u stog
pop(&first, &second)//izbacujemo za stoga zadnji element koji je usa LIFO last in firs out

a)-alloc prostor + inicijalizacija
  newEl=NULL
  Position
  newEl->next=head->next (koristi q kad si u funkcijei saljes head al koristi ga kao q u funkciji)
  head->next=newEl;

b) first = head->next;
   second = first->next

        7                 3             4(ka vrijensotil iste)
  first->number = (first->number,second->number,"%c")(kad racunamo nekakvu operaciju izmedu dva clana spremamo je u prvi il drugi ka da stedimo nemoramo ekakkav temp)
  pop(&first,&second);u popu oslobdamo memorije oog clana lsite kojeg nismo iskoritsitli ako imamo 3 i 4 tobi redimo bia 4 drugi clan jese u prvi zbroj spremija
  first->next =second->next
  second->next = NULL
  free(second);
c)samo ka trash value jel vidit
     
VRACAj EXIT_SUCCES ili EXIT_FAULIRE iz funkcija lakse nego 0 -1 -2.....
