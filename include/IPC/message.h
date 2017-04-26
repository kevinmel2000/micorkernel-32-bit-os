#ifndef MSG_H
#define MSG_H
#define SEND         0x1 
#define RECEIVE      0x2
#define ROUND_TRIP   0x3
#define WAIT_SEND    0x4
#define WAIT_RECEIVE 0x5
#define ANY          0xffffffff
#define MSG_BUF_SIZE 128
#define NEED_NOTHING 0x8
#define HAD_SIGNAL   0x9000
#define INTERRUPT    0x9100
struct message
{
  unsigned int type;
  unsigned int src;
  unsigned int dst;
  unsigned int function;
  unsigned int stright_msg;
  unsigned int return_value;
  void *translate_msg;
};
struct message_manager
{
  unsigned int status;
  unsigned int want_receive_from;
  unsigned int want_send_to;
  struct message *msg;
};
struct message_buffer
{
  struct message msg;
  struct message_buffer *next;
};
#define IS_EMPTY(buffer) (buffer==NULL)? 1:0
extern void send_recv(struct message *msg);
extern void sys_send_message(struct message *msg);
extern void sys_receive_message(struct message *msg);
extern void copy_message(struct message *src,struct message *dst);
extern int save_message(struct message *msg);
extern struct message *get_message(struct message *msg);
extern void clear_message(int pid);
extern void receive_message(struct message *msg);
extern void send_message(struct message *msg);
extern void round_message(struct message *msg);
struct message_buffer *message_list;
struct message_buffer *message_empty;
struct message_buffer *message_send;
#endif
