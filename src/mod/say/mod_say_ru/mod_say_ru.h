/* Contributor(s):
 *
 * Boris Buklov (BBV) <buklov@mail.ru>
 *
 * mod_say_ru.c -- Say for Russian
 *
 */


//слишком много падежей поэтому пришлось сделать массив значений
//пример таблицы склонений для 1 ниже по ссылкам
//http://ru.wiktionary.org/wiki/%D0%BE%D0%B4%D0%B8%D0%BD
//http://ru.wiktionary.org/wiki/%D0%BF%D0%B5%D1%80%D0%B2%D1%8B%D0%B9

typedef enum {
	million,
	thousand,
	zero,
	empty
} unit_t;

//тип числа порядковое или количественное
typedef enum {
	nominativus,				// именительный Кто? Что?
	genitivus,					// Родительный  Кого? Чего?
	dativus,					// дательный    Кого? Что?
	accusativus_a,				// Винительный  Кого? animate - одушевлённый
	accusativus_i,				// Винительный  Что?  inanimate - неодушевлённый
	instrumentalis,				// Творительный Кем? Чем?
	prepositive					// Предложный   О ком? О чём?
} casus_t;


typedef enum {
	//количественный
	male_c,						//мужского пола
	it_c,						//среднего
	female_c,					//женского
	plural_c,					//множественное число
	//порядковый
	male_h,						//порядковое мужского пола
	it_h,						//порядковое среднего
	female_h,					//порядковое женского
	plural_h					//порядковое множественное число
} say_type_t;



struct say_t {
	char *num[8];
	char *thousand[14];
	char *million[14];
	int all;
};


//именительный 
//именительный количественный/числительное

//мужской род родительный падеж

//первая строка описывае как произносить цифры от 0 до 5 потом пристаставка и окончания для остальных
// тоже самое для тысяч и миллионов, только ещё с произношением тысяч и миллионов в вонце так же окончание для цифр
// как произносить тысячи и миллионы берёт с цифры 5

#define m_00 {\
	{"0","1","2","3","4","5","",""},\
	{"","","1f","thousand","2f","thousands-i","3","thousands-i","4","thousands-i","5","thousands","",""},\
	{"","","1","million","2","million-a","3","million-a","4","million-a","5","millions","",""},\
	0,\
}\
								//средний род именительный падеж
#define m_01 {\
	{"0","1n","2","3","4","5","",""},\
	{"","","1f","thousand","2f","thousands-i","3","thousands-i","4","thousands-i","5","thousands","",""},\
	{"","","","million","2","million-a","3","million-a","4","million-a","5","millions","",""},\
	0,\
}\

//женский
#define m_02 {\
	{"0","1f","2f","3","4","5","",""},\
	{"","","1f","thousand","2f","thousands-i","3","thousands-i","4","thousands-i","5","thousands","",""},\
	{"","","1","million","2","million-a","3","million-a","4","million-a","5","millions","",""},\
	0,\
}\

//множественное число
#define m_03 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
#define m_04 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_05 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_06 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_07 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\



//Родительный  Кого? Чего?
//количественный/числительное
//мужской
#define m_10 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_11 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//женский
#define m_12 {\
	{"0","h-1xx","h-2xx","h-3xx","h-4xx","h-5xx","h-","xx"},\
	{"","","1xx","thousands-i","h-2xx","thousands","h-3xx","thousands","h-4xx","thousands","h-5xx","thousands","h-","xx"},\
	{"","","1x","million-a","h-2xx","millions","h-3xx","millions","h-4xx","millions","h-5xx","millions","h-","xx"},\
	0,\
}\

//множественное число
#define m_13 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
//родительный
#define m_14 {\
	{"h-0x","h-1x","h-2x","h-3x","h-4x","h-5x","h-","x"},\
	{"","","","h-thousandx","h-2xx","h-thousandx","h-3xx","h-thousandx","h-4xx","h-thousandx","h-5xx","h-thousandx","h-","xx"},\
	{"","","","h-millionx","h-2xx","h-millionx","h-3xx","h-millionx","h-4xx","h-millionx","h-5xx","h-millionx","h-","xx"},\
	0,\
}\
								//средний
#define m_15 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_16 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_17 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\



//дательный    Кого? Что? 
//количественный/числительное
//мужской
#define m_20 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_21 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//женский
#define m_22 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_23 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
#define m_24 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_25 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_26 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_27 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\



//Винительный  Кого? animate - одушевлённый 
//количественный/числительное
//мужской
#define m_30 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_31 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//женский
#define m_32 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_33 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
#define m_34 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_35 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_36 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_37 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\



//Винительный  Что?  inanimate - неодушевлённый 
//количественный/числительное
//мужской
#define m_40 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_41 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//женский
#define m_42 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_43 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
#define m_44 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_45 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_46 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_47 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\



//Творительный Кем? Чем? 
//количественный/числительное
//мужской
#define m_50 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_51 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//женский
#define m_52 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_53 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
#define m_54 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_55 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_56 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_57 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\



//Предложный   О ком? О чём? 
//количественный/числительное
//мужской
#define m_60 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_61 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//женский
#define m_62 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_63 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
#define m_64 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_65 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_66 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_67 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\



//именительный 
//именительный количественный/числительное
//мужской
#define m_60 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_61 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//женский
#define m_62 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_63 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//порядковый
//мужской
#define m_64 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//средний
#define m_65 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
								//женский
#define m_66 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\

//множественное число
#define m_67 {\
	{"","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	{"","","","","","","","","","","","","",""},\
	0,\
}\
