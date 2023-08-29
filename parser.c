#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define likely(expression) __builtin_expect(!!(expression), 1)
#define unlikely(expression) __builtin_expect(!!(expression), 0)

struct including {
	unsigned char sign;
	int value;
};
int ile = 0;

int parse(char *buffer) {
	int length = strlen(buffer);
	if (length > 5000)
		return -1;

	char valid[256];
	memset(valid, 0, 256);
	char tmpchar[4] = {'\n', '\r', '\t', ' '};
	for (int i = 0; i < 4; i++) {
		valid[tmpchar[i]] = 1;
	}
	enum all {
		BEGIN,
		WAIT,
		KEY,
		MIDDLE,
		AFTERMIDDLE,
		SKIP,
		AFTER,
		VALUE,
		END,
		BADEND
	};

	struct including link_array[7][3] = {{{'{', WAIT}, {'[', SKIP}, {0, -1}}, // begin
					{{'"', KEY}, {'}', END}, {0, -1}}, // wait
					{{'"', MIDDLE}, {0, -1}, {0, -1}}, // key
					{{':', AFTERMIDDLE}, {0, -1}, {0, -1}}, // middle
					{{'"', VALUE}, {'{', SKIP}, {'[', SKIP}}, // aftermiddle	
					{{0, -1}, {0, -1}, {0, -1}}, // skip
					{{'}', END}, {',', WAIT}, {0, -1}}}; //after
					
	int link[7][256];
	for(int i = 0; i < 7; i++) {
		for(int a = 0; a < 256; a++) {
			link[i][a] = BADEND;
		}
	}
	for(int i = 0; i < 7; i++) {
		for(int a = 0; a < 3; a++) {
			if(link_array[i][a].value == -1)
				break;
			link[i][link_array[i][a].sign] = link_array[i][a].value;
		}
	}
	
		
	for(int i = 0; i < 10; i++) {
		link[AFTERMIDDLE][i + 48] = VALUE;
	}
	link[AFTERMIDDLE]['t'] = VALUE;
	link[AFTERMIDDLE]['f'] = VALUE;
	link[AFTERMIDDLE]['-'] = VALUE;
	link[WAIT]['{'] = SKIP;
	link[WAIT]['['] = SKIP;

	enum forValues {
		N,
		S,
		T,
		F,
		BEND
	};
	int inValue_link[256];
		for(int a = 0; a < 256; a++) {
			inValue_link[a] = BEND;
		}
	inValue_link['"'] = S;
	inValue_link['t'] = T;
	inValue_link['f'] = F;
	inValue_link['-'] = N;
	inValue_link['+'] = N;
	for(int i = 0; i < 10; i++) {
		inValue_link[i + 48] = N;
	}
	enum inValues {
		PM,
		NR,
		FRAC,
		EXPPM,
		EXP,
		BREAK,
		BADBREAK
	};

	struct including inNumber_link_array[3][3] = {{{0, -1}, {0, -1}, {0, -1}},
					{{'.', FRAC}, {'e', EXPPM}, {'E', EXPPM}},
					{{'e', EXPPM}, {'E', EXPPM}, {0, -1}}};

	int number_links[6][256];
	for(int i = 0; i < 6; i++) {
		for(int a = 0; a < 256; a++) {
			number_links[i][a] = BADBREAK;
		}
	}
	for(int i = 0; i < 3; i++) {
		for(int a = 0; a < 3; a++) {
			if(inNumber_link_array[i][a].value == -1)
				break;
			number_links[i][inNumber_link_array[i][a].sign] = inNumber_link_array[i][a].value;
		}
	}
	for(int i = 0; i < 4; i++) {
		number_links[NR][tmpchar[i]] = BREAK;
		number_links[FRAC][tmpchar[i]] = BREAK;
		number_links[EXP][tmpchar[i]] = BREAK;
	}
	number_links[NR]['}'] = BREAK;
	number_links[FRAC]['}'] = BREAK;
	number_links[EXP]['}'] = BREAK;
	number_links[NR][','] = BREAK;
	number_links[FRAC][','] = BREAK;
	number_links[EXP][','] = BREAK;
	for(int i = 0; i < 10; i++) {
		number_links[EXPPM][i + '0'] = EXP;
		number_links[PM][i + '0'] = NR;
	}

	char *keystring = malloc(128);
	char *valuestring = malloc(128);
	long long finalvalue = 0, start, len;
	int next_state = BEGIN;
	int index = 0;
	int if_from_aftermiddle = 0;
	char closing_brackets[256];
	closing_brackets['{'] = '}';
	closing_brackets['['] = ']';
	char last[1024];
	memset(last, 0, 1024);
	int lastcount = -1;
	char look = 0;
	int next_value_state = 0;
	int negative = 1;
	int negexp = 0;
	int fraclist[64];
	int exp = 0;
	while (likely(index < length)) {
		if (unlikely(valid[buffer[index]])) {
			index++;
			continue;
		}
		switch (next_state) {
			case BEGIN: //begin
				next_state = link[BEGIN][buffer[index]];
			break;

			case WAIT: //wait
			if((buffer[index] == '}' || buffer[index] ==']') && lastcount >= 0) {
				next_state = AFTER;
				continue;
			}
				next_state = link[WAIT][buffer[index]];
			break;

			case SKIP: // skip
				if(!if_from_aftermiddle) {
					index--;
				} else
					if_from_aftermiddle = 0;
				lastcount++;
				last[lastcount] = closing_brackets[buffer[index]];
				look = last[lastcount];	
				next_state = WAIT;
			break;

			case KEY: //key
				memset(keystring, 0, 128);
				start = index;
				while (link[KEY][buffer[index]] != MIDDLE) {
					if(buffer[index] == 92) {
						buffer[index] = buffer[index + 1];
						buffer[index + 1] = 0;
					}
					index++;
				}
					
				len = index - start;
				memcpy(keystring, &buffer[start], len);
				next_state = link[KEY][buffer[index]];
			break;

			case MIDDLE: // middle
				next_state = link[MIDDLE][buffer[index]];
			break;

			case AFTERMIDDLE:
				if(link[AFTERMIDDLE][buffer[index]] == SKIP)
				if_from_aftermiddle = 1;
				next_state = link[AFTERMIDDLE][buffer[index]];
				continue;
			break;

			case VALUE:
				switch (inValue_link[buffer[index]]) {
					case N:
						next_value_state = PM;
						while(next_value_state != BREAK) {
							switch (next_value_state) {
								case PM:
									negative = 1;
									if(buffer[index] == '-') {
										index++;
										negative = -1;
									} else if(buffer[index] == '+') {
										index++;
									}
									next_value_state = number_links[PM][buffer[index]];

								break;

								case NR:
									finalvalue = 0;
									while(buffer[index] >= '0' && buffer[index] <= '9') {
										finalvalue = finalvalue * 10 + buffer[index] - '0';
										index++;
									}
									next_value_state = number_links[NR][buffer[index]];
								break;

								case FRAC:
									index++;
									memset(fraclist, 0 , 64);
									int now = index;
									while(buffer[index] >= '0' && buffer[index] <= '9') {
										if(index - now < 64)
											fraclist[index - now] = buffer[index] - '0';
										index++;
									}
									next_value_state = number_links[FRAC][buffer[index]];
								break;

								case EXPPM:
									index++;
									negexp = 0;
									if(buffer[index] == '-') {
										negexp = 1;
										index++;
									} else if (buffer[index] == '+') {
										index++;
									}
									next_value_state = number_links[EXPPM][buffer[index]];
								break;

								case EXP:
								
									exp = 0;
									while(buffer[index] >= '0' && buffer[index] <= '9') {
										exp = exp * 10 + buffer[index] - '0';
										index++;
									}
									next_value_state = number_links[EXP][buffer[index]];
								break;

								case BADBREAK:
									return -1;
								break;
							}
						}
						finalvalue *= negative;
						if(negexp == 1) {
							for(int i = 0; i < exp; i++) {
								finalvalue /= 10;
							}
						} else {
							for(int i = 0; i < exp; i++) {
								finalvalue *= 10;
								if(i < 64)
									finalvalue += fraclist[i];
							}
						}
						printf("%s : %lld\n", keystring, finalvalue);
						next_state = AFTER;
						continue;
					break;

					case S:
						memset(valuestring, 0, 128);
						index++;
						start = index;
						while (unlikely(buffer[index] != '"')) {
							if(buffer[index] == 92) {
								buffer[index] = buffer[index + 1];
								buffer[index + 1] = 0;
							}
							index++;
						}
							
						len = index - start;
						memcpy(valuestring, &buffer[start], len);
						next_state = AFTER;
						if(lastcount < 0)
							printf("%s : %s\n", keystring, valuestring);
					break;

					case T:
						if(likely(index + 3 < length)) {
							if(likely(buffer[index + 1] == 'r') && likely(buffer[index + 2] == 'u') && 
							   likely(buffer[index + 3] == 'e')) {
								finalvalue = 1;
								if(lastcount < 0)
									printf("%s : %lld\n", keystring, finalvalue);
								next_state = AFTER;
								index += 3;
								break;
							}
						}
						free(keystring);
						free(valuestring);
						return -1;
					break;

					case F:
						if(likely(index + 4 < length)) {
							if(likely(buffer[index + 1] == 'a') && likely(buffer[index + 2] == 'l') &&
							   likely(buffer[index + 3] == 's') && likely(buffer[index + 4] == 'e')) {
								finalvalue = 0;
								if(lastcount < 0)
									printf("%s : %lld\n", keystring, finalvalue);
								next_state = AFTER;
								index += 4;
								break;
							}
						}
						free(keystring);
						free(valuestring);
						return -1;
					break;

					case BEND:
						free(keystring);
						free(valuestring);
						return -1;
					break;
				}
			break;

			case AFTER:

				if(lastcount >= 0) {
					if(look == buffer[index]) {
						lastcount--;
						look = last[lastcount];
						next_state = AFTER;
						break;
					}
				}
				next_state = link[AFTER][buffer[index]];
				
			break;

			case END:
				free(keystring);
				free(valuestring);
				return 0;
			
			case BADEND:
				free(keystring);
				free(valuestring);
				return -1;
		}
		if (unlikely(next_state != END) && unlikely(next_state != BADEND))
			index++;
	}
	free(keystring);
	free(valuestring);
	return -1;

}

int main() {
	char *ez = malloc(1000);
	int i = 0;
	while(1) {
		if(i > 0 && ez[i - 1] == 0)
			break;
		scanf("%c", &ez[i]);
		i++;
	}
	printf("%d", parse(ez));
}
