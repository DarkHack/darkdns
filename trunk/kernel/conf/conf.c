/*
 * conf.c
 *
 *  Created on: 19 oct. 2012
 *      Author: chapuis
 */
#include "conf.h"

static char* DARKDNS_KERNEL_CONF_TAB [] = {
  "psql_info"
 ,"srv_port_tcp"
 ,"srv_port_udp"
 ,"log_file"
 ,"log_level"
};

conf* darkdns_kernel_conf_init() {
 //printf("ici nous avons %s\n",DARKDNS_KERNEL_CONF_CONFILE);
 conf* self= darkdns_kernel_memory_alloc(sizeof *self);

 if (self == NULL)
  fprintf(stderr,"darkdns_kernel_conf_init no more memory for self\n");
 else {
  self->params= darkdns_kernel_memory_alloc(sizeof *self->params);
  if (self->params==NULL) {
   darkdns_kernel_memory_free(self);
   self=0x00;
   printf("darkdns_kernel_conf_init no more memory for self->params\n");
  }else {
   self->read=darkdns_kernel_conf_read;
   self->write_exemple=darkdns_kernel_conf_write_exemple;
   self->absolute_path=darkdns_kernel_conf_get_path;
   self->params->conf_file=self->absolute_path("./darkdns.conf");
  }
 }
 return self;
}
void darkdns_kernel_conf_free(conf* myConf) {
 if (myConf->params!=NULL) {
  darkdns_kernel_memory_free(myConf->params->conf_file);
  myConf->params->conf_file=0x00;
  darkdns_kernel_memory_free(myConf->params->log_file);
  myConf->params->log_file=0x00;
  darkdns_kernel_memory_free(myConf->params);
  myConf->params=0x00;
 }
 darkdns_kernel_memory_free(myConf);
 myConf=0x00;
}

char* darkdns_kernel_conf_get_path(char *file)
{
 Buffer* bufPath = darkdns_kernel_buffer_init(sizeof(char));
 char *path_to_file=0x00;

 if (bufPath==NULL)
  printf("darkdns_kernel_conf_get_path no more memory for bufPath\n");
 else {
  ///*
  if ((char)(*file)==(char)'/')
   bufPath->from_one(bufPath,file,"\0",1);
  else {
   bufPath->from_one(bufPath,getcwd(0,128),"\0",0);
   bufPath->from_one(bufPath,(file+1),"\0",1);
  }
  //*/
  path_to_file=(char*)bufPath->in_one(bufPath,"\0");
 }
 //*/
 darkdns_kernel_buffer_free(bufPath);
 bufPath=0x00;
 return path_to_file;
}

void darkdns_kernel_conf_read(conf *myConf)
{
 FILE *fp;
 Buffer *bLine=darkdns_kernel_buffer_init(sizeof(char));
 char c;
 if (bLine==NULL) {
  fprintf(stderr,"Error darkdns_kernel_conf_read no more memory for buffer\n");
 }else {
  // read conf file
  if ((fp = fopen(myConf->params->conf_file,"a+")) == NULL)
   fprintf(stderr,"Access impossible to %s\n",myConf->params->conf_file);
  else {
   // size of conf file
   fseek(fp,0,SEEK_END);
   // like an empty?
   if (ftell(fp)<=2) {
    fclose(fp);
    // write an exemple of conf
    myConf->write_exemple(myConf);
    if ((fp = fopen(myConf->params->conf_file,"a+")) == NULL)
     fprintf(stderr,"Access impossible to %s\n",myConf->params->conf_file);
   }
   if (fp!=NULL) {
    // put in begin of configuration file
    fseek(fp,0,SEEK_SET);
    // read every line
    do {
     c=(char)fgetc(fp);
     if (c!='\0' && c!='\n' && c!='#' && c!=EOF ) {
      if (!bLine->write(bLine,&c))
       fprintf(stderr,"darkdns_kernel_conf_read ecriture buffer echouer\n");
     }
     if (c=='#') {
      while(c!='\n' && c!=EOF)
       c=(char)fgetc(fp);
     }
     if (c=='\n') {
      c=';';
      if (!bLine->write(bLine,&c))
       printf("darkdns_kernel_conf_read ecriture buffer echouer\n");
      if (bLine->size>0)
       darkdns_kernel_conf_analyse_line(myConf,bLine);
      bLine->clear(bLine);
     }
    } while(c!=EOF);
    fclose(fp);
   }
  }
 }
 darkdns_kernel_buffer_free(bLine);
 //printf("name : %s\t\tvalue : %s\n",(char*)bConfName.in_one(&bConfName,&endCaract),(char*)bConfValue.in_one(&bConfValue,&endCaract));
 //printf("%s",(char*)bUtilConf.in_one(&bUtilConf,&endCaract));
 //if (fp!=NULL) fclose(fp);
 /*
 printf("srv_port_tcp\t:%d\n",myConf->params->port_tcp);
 printf("srv_port_udp\t:%d\n",myConf->params->port_udp);
 printf("log_level\t:%d\n",myConf->params->log_level);
 printf("log_file\t:%s\n",myConf->params->log_file);
 printf("conf_file\t:%s\n",myConf->params->conf_file);
 */
 return ;
}

int darkdns_kernel_conf_analyse_line(conf *myConf,Buffer *bUtilConf)
{
 Buffer
  *bConfName=darkdns_kernel_buffer_init(sizeof(char))
 ,*bConfValue=darkdns_kernel_buffer_init(sizeof(char));
 char *c,stateNameOrValue,*bufName,*bufValue;

 //printf("bUtilConf :%s\n",(char*)bUtilConf->in_one(bUtilConf,&endCaract));
 bUtilConf->rewind(bUtilConf);


 stateNameOrValue=0;
 while((c = bUtilConf->read(bUtilConf)) != NULL) {
  if (*c==';') {
   stateNameOrValue=0;
   // has an name or an value ?
   if (bConfName->size>0 || bConfValue->size >0 ) {
    bufName=(char*)bConfName->in_one(bConfName,"\0");
    bufValue=(char*)bConfValue->in_one(bConfValue,"\0");
    //wich params is?
    ///*
    if (darkdns_kernel_string_compare(bConfName,DARKDNS_KERNEL_CONF_TAB[psql_info]))
     myConf->params->psql_info=bConfValue->in_one(bConfValue,"\0");
    else if (darkdns_kernel_string_compare(bConfName,DARKDNS_KERNEL_CONF_TAB[port_tcp]))
     myConf->params->port_tcp=darkdns_kernel_string_to_int(bConfValue);
    else if (darkdns_kernel_string_compare(bConfName,DARKDNS_KERNEL_CONF_TAB[port_udp]))
     myConf->params->port_udp=darkdns_kernel_string_to_int(bConfValue);
    else if (darkdns_kernel_string_compare(bConfName,DARKDNS_KERNEL_CONF_TAB[log_level]))
     myConf->params->log_level=darkdns_kernel_string_to_int(bConfValue);
    else if (darkdns_kernel_string_compare(bConfName,DARKDNS_KERNEL_CONF_TAB[log_file]))
     myConf->params->log_file=myConf->absolute_path(bufValue);
    else //*/
     printf("params unknow => name : %s\t\tvalue : %s\n",bufName,bufValue);
    darkdns_kernel_memory_free(bufName);
    darkdns_kernel_memory_free(bufValue);
   }
  }else if (*c=='=' && stateNameOrValue==0) {
    stateNameOrValue=1;
  } else {
   if (stateNameOrValue)
   {
    bConfValue->write(bConfValue,c);
   } else
   {
    if (*c!=' ')
    bConfName->write(bConfName,c);
   }
  }
 }
 darkdns_kernel_buffer_free(bConfName);
 darkdns_kernel_buffer_free(bConfValue);
 return 0;
}


int darkdns_kernel_conf_write_exemple(conf *myConf)
{
 FILE *fp;
 if ((fp = fopen(myConf->params->conf_file,"a+")) == NULL)
 {
  fprintf(stderr,"Access impossible to %s\n",myConf->params->conf_file);
  return 0;
 }
 fprintf(fp,"# -----------------------------\n");
 fprintf(fp,"# DarkDns configuration file   \n");
 fprintf(fp,"# -----------------------------\n");
 fprintf(fp,"\n\n");
 fprintf(fp,"# sql connexion\n");
 fprintf(fp,"psql_info = host = 127.0.0.1 dbname = postgres user = webroot password = *[webroot]*;\n");
 fprintf(fp,"# serveur\n");
 fprintf(fp,"srv_port_tcp = 53;\n");
 fprintf(fp,"srv_port_udp = 53;\n");
 fprintf(fp,"\n\n");
 fprintf(fp,"# log\n");
 fprintf(fp,"log_file =./darkdns.log;# default : /var/log/darkdns.log\n");
 fprintf(fp,"log_level = 5 # default : 0\n");
 fflush(fp);
 fclose(fp);
 return 1;
}
