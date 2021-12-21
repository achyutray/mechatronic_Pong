/*
 * CAN.h
 *
 * Created: 13.10.2021 11:25:03
 *  Author: mbmartin
 */ 


#ifndef CAN_H_
#define CAN_H_
bool receive_flag;

typedef struct CAN_messages{
	uint8_t id;
	uint8_t length;
	uint8_t data[8];
} CAN_message;

CAN_message reception_container;

CAN_message transmission_container;

bool playing_game_flag;
int score;
uint8_t difficulty_indicator;	// 0 -> easy, 1 -> medium, 2 -> hard

void CAN_init();

void CAN_send_message(CAN_message *message);

void send_controls(void);

void send_difficulty(void);

void can_debugging_test_send(void);

void CAN_receive_message(CAN_message* message);

#endif /* CAN_H_ */