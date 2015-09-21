/*
 * logger.c
 *
 *  Created on: 1 nov. 2012
 *      Author: chapuis
 */
#include "logger.h"

Log* darkdns_kernel_logger_init(conf_params *params)
{
 Log* self= darkdns_kernel_memory_alloc(sizeof *self);

 if (self == NULL)
  printf("darkdns_kernel_logger_init no more memory for create an logger object");
 else {
  self->childs=darkdns_kernel_memory_alloc(sizeof(LogChildChaine));
  if (self->childs == NULL) {
   printf("darkdns_kernel_logger_init no more memory for create an logger object with child");
   darkdns_kernel_logger_free(self);
   self=0x00;
  }else {
   self->childs->me=0x00;
   self->childs->next=0x00;
   // attributes
   self->params=params;
   // methode
   self->child=darkdns_kernel_logger_child;
  }
 }
 return self;
}
void darkdns_kernel_logger_free(Log* myLog) {
 darkdns_kernel_logger_child_free(myLog->childs);
 darkdns_kernel_memory_free(myLog);
 myLog=0x00;
}

LogChild* darkdns_kernel_logger_child(Log *myLog,char *className)
{
 LogChildChaine *logChildPos;

 logChildPos=myLog->childs;
 while(logChildPos->next!=0x00)
  logChildPos=logChildPos->next;
 if (logChildPos->me!=0x00) {
  logChildPos->next=darkdns_kernel_memory_alloc(sizeof(LogChildChaine));
  logChildPos->next->me=0x00;
  logChildPos->next->next=0x00;
  logChildPos=logChildPos->next;
 }
 logChildPos->me=darkdns_kernel_memory_alloc(sizeof(LogChild));
 logChildPos->me->class_name=darkdns_kernel_buffer_init(sizeof(char));
 logChildPos->me->class_name->from_one(logChildPos->me->class_name,className,"\0",0);
 logChildPos->me->methode_name=darkdns_kernel_buffer_init(sizeof(char));
 logChildPos->me->info=darkdns_kernel_buffer_init(sizeof(char));
 logChildPos->me->parent=myLog;
 logChildPos->me->write=darkdns_kernel_logger_write;

 return logChildPos->me;
}
void darkdns_kernel_logger_child_free(LogChildChaine* logChildPos) {
 if (logChildPos->next!=NULL)
  darkdns_kernel_logger_child_free(logChildPos->next);
 logChildPos->next=0x00;
 if (logChildPos->me!=NULL) {
  if (logChildPos->me->class_name)
   darkdns_kernel_buffer_free(logChildPos->me->class_name);
  logChildPos->me->class_name=0x00;
  if (logChildPos->me->methode_name)
   darkdns_kernel_buffer_free(logChildPos->me->methode_name);
  logChildPos->me->methode_name=0x00;
  if (logChildPos->me->info)
   darkdns_kernel_buffer_free(logChildPos->me->info);
  logChildPos->me->info=0x00;
  logChildPos->me->parent=0x00;
  darkdns_kernel_memory_free(logChildPos->me);
 }
 logChildPos->me=0x00;
 darkdns_kernel_memory_free(logChildPos);
 logChildPos=0x00;
}

void darkdns_kernel_logger_write(LogChild *myLogChild,char *methode,char *info)
{
 FILE *fp;
 Buffer *bLine=darkdns_kernel_buffer_init(sizeof(char));
 time_t timestamp = time(NULL);
 struct tm * t= localtime(&timestamp);
 char *className=myLogChild->class_name->in_one(myLogChild->class_name,"\0");
 // read conf file
 if ((fp = fopen(myLogChild->parent->params->log_file,"a+")) == NULL)
  fprintf(stderr,"Access impossible to %s\n",myLogChild->parent->params->log_file);
 else {
  // size of conf file
  fseek(fp,0,SEEK_END);
  fprintf(fp,"%04u-%02u-%02u|%02u:%02u:%02u|",(1900+t->tm_year),t->tm_mon,t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec);
  fprintf(fp,"%s => %s|info:\n%s\n",className,methode,info);
  fflush(fp);
  fclose(fp);
 }
 darkdns_kernel_buffer_free(bLine);
 darkdns_kernel_memory_free(className);
 return ;
}
