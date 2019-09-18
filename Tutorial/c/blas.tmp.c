 #define VDEBUG (1) 
 

#include <stdio.h>  // TODO remove all these includes, define all function signatures and types manually
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h> // for va_list
#include <inttypes.h>  // int64_t etc
#include <string.h> // memcpy

#ifndef _WIN32
#include <ctype.h>
#include <locale.h> // tolower
#include <sys/time.h>
#include <unistd.h> // sleep	
#endif

#ifdef __APPLE__
#include <libproc.h> // proc_pidpath
#include <execinfo.h> // backtrace and backtrace_symbols_fd
#endif

#ifdef __linux__
#include <execinfo.h> // backtrace and backtrace_symbols_fd
#pragma weak backtrace
#pragma weak backtrace_symbols_fd
#endif

#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h> // os__wait uses wait on nix
#endif


#define EMPTY_STRUCT_DECLARATION
#define OPTION_CAST(x) (x)

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// must be included after <windows.h>
#include <shellapi.h>

#include <io.h> // _waccess
#include <fcntl.h> // _O_U8TEXT
#include <direct.h> // _wgetcwd
//#include <WinSock2.h>
#ifdef _MSC_VER
// On MSVC these are the same (as long as /volatile:ms is passed)
#define _Atomic volatile

// MSVC cannot parse some things properly
#undef EMPTY_STRUCT_DECLARATION
#define EMPTY_STRUCT_DECLARATION void *____dummy_variable;
#undef OPTION_CAST
#define OPTION_CAST(x)
#endif

void pthread_mutex_lock(HANDLE *m) {
	WaitForSingleObject(*m, INFINITE);
}

void pthread_mutex_unlock(HANDLE *m) {
	ReleaseMutex(*m);
}
#else
#include <pthread.h>
#endif

//================================== TYPEDEFS ================================*/

typedef unsigned char byte;
typedef unsigned int uint;
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef uint32_t rune;
typedef float f32;
typedef double f64;
typedef unsigned char* byteptr;
typedef int* intptr;
typedef void* voidptr;
typedef struct array array;
typedef struct map map;
typedef array array_string;
typedef array array_int;
typedef array array_byte;
typedef array array_uint;
typedef array array_float;
typedef array array_f32;
typedef array array_f64;
typedef map map_int;
typedef map map_string;
#ifndef bool
	typedef int bool;
	#define true 1
	#define false 0
#endif

//============================== HELPER C MACROS =============================*/

#define _PUSH(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array__push(arr, &tmp);}
#define _PUSH_MANY(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array__push_many(arr, tmp.data, tmp.len);}
#define _IN(typ, val, arr) array_##typ##_contains(arr, val)
#define _IN_MAP(val, m) map__exists(m, val)
//#define ALLOC_INIT(type, ...) (type *)memdup((type[]){ __VA_ARGS__ }, sizeof(type))

//================================== GLOBALS =================================*/
byteptr g_str_buf;
int load_so(byteptr);
void reload_so();
void init_consts();


 int g_test_ok = 1; 
 /*================================== FNS =================================*/
 this line will be replaced with definitions
 array new_array(int mylen, int cap, int elm_size) {
 
array arr= (array) { .len =  mylen , .cap =  cap , .element_size =  elm_size , .data =  v_malloc ( cap * elm_size ) } ;
 
return  arr ;
 
 
 }
 array _make(int len, int cap, int elm_size) {
 
return  new_array ( len , cap , elm_size ) ;
 
 
 }
 array new_array_from_c_array(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  v_malloc ( cap * elm_size ) } ;
 
 memcpy ( arr .data ,  c_array ,  len * elm_size ) ;
 
return  arr ;
 
 
 }
 array new_array_from_c_array_no_alloc(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  c_array } ;
 
return  arr ;
 
 
 }
 array array_repeat(void* val, int nr_repeats, int elm_size) {
 
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  elm_size , .data =  v_malloc ( nr_repeats * elm_size ) } ;
 
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * elm_size ,  val ,  elm_size ) ;
 
 }
 ;
 
return  arr ;
 
 
 }
 void array_sort_with_compare(array* a, void* compare) {
 
 qsort ( a ->data ,  a ->len ,  a ->element_size ,  compare ) ;
 
 
 }
 void array_insert(array* a, int i, void* val) {
 
 if ( i >= a ->len ) {
 
 _panic_debug (79, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"insert"),  tos2((byte*)"array.insert: index larger than length") ) ;
 
 }
 ;
 
 array__push( a , val ) ;
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + ( i + 1 ) * size , (byte*) a ->data + i * size ,  ( a ->len - i ) * size ) ;
 
 array_set( a , i , val ) ;
 
 
 }
 void array_prepend(array* a, void* val) {
 
 array_insert( a , 0 , val ) ;
 
 
 }
 void array_delete(array* a, int idx) {
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + idx * size , (byte*) a ->data + ( idx + 1 ) * size ,  ( a ->len - idx ) * size ) ;
 
 a ->len -- ;
 
 a ->cap -- ;
 
 
 }
 void* array__get(array a, int i) {
 
 if ( i < 0  ||  i >= a .len ) {
 
 _panic_debug (100, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"_get"),  _STR("array index out of range: %d/%d", i, a .len) ) ;
 
 }
 ;
 
return (byte*) a .data + i * a .element_size ;
 
 
 }
 void* array_first(array a) {
 
 if ( a .len == 0 ) {
 
 _panic_debug (107, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"first"),  tos2((byte*)"array.first: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + 0 ;
 
 
 }
 void* array_last(array a) {
 
 if ( a .len == 0 ) {
 
 _panic_debug (114, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"last"),  tos2((byte*)"array.last: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + ( a .len - 1 ) * a .element_size ;
 
 
 }
 array array_left(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , 0 , n ) ;
 
 
 }
 array array_right(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , n , s .len ) ;
 
 
 }
 array array_slice(array s, int start, int _end) {
 
int end= _end ;
 
 if ( start > end ) {
 
 _panic_debug (136, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"slice"),  _STR("invalid slice index: %d > %d", start, end) ) ;
 
 }
 ;
 
 if ( end > s .len ) {
 
 _panic_debug (139, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"slice"),  _STR("runtime error: slice bounds out of range (%d >= %d)", end, s .len) ) ;
 
 }
 ;
 
 if ( start < 0 ) {
 
 _panic_debug (142, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"slice"),  _STR("runtime error: slice bounds out of range (%d < 0)", start) ) ;
 
 }
 ;
 
int l= end - start ;
 
array res= (array) { .element_size =  s .element_size , .data = (byte*) s .data + start * s .element_size , .len =  l , .cap =  l } ;
 
return  res ;
 
 
 }
 void array_set(array* a, int idx, void* val) {
 
 if ( idx < 0  ||  idx >= a ->len ) {
 
 _panic_debug (157, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/array.v"), tos2((byte *)"builtin"), tos2((byte *)"set"),  _STR("array index out of range: %d / %d", idx, a ->len) ) ;
 
 }
 ;
 
 memcpy ((byte*) a ->data + a ->element_size * idx ,  val ,  a ->element_size ) ;
 
 
 }
 void array__push(array* arr, void* val) {
 
 if ( arr ->len >= arr ->cap - 1 ) {
 
int cap= ( arr ->len + 1 ) * 2 ;
 
 if ( arr ->cap == 0 ) {
 
 arr ->data  =  v_malloc ( cap * arr ->element_size ) ;
 
 }
  else { 
 
 arr ->data  =  realloc ( arr ->data ,  cap * arr ->element_size ) ;
 
 }
 ;
 
 arr ->cap  =  cap ;
 
 }
 ;
 
 memcpy ((byte*) arr ->data + arr ->element_size * arr ->len ,  val ,  arr ->element_size ) ;
 
 arr ->len ++ ;
 
 
 }
 void array__push_many(array* arr, void* val, int size) {
 
 if ( arr ->len >= arr ->cap - size ) {
 
int cap= ( arr ->len + size ) * 2 ;
 
 if ( arr ->cap == 0 ) {
 
 arr ->data  =  v_malloc ( cap * arr ->element_size ) ;
 
 }
  else { 
 
 arr ->data  =  realloc ( arr ->data ,  cap * arr ->element_size ) ;
 
 }
 ;
 
 arr ->cap  =  cap ;
 
 }
 ;
 
 memcpy ((byte*) arr ->data + arr ->element_size * arr ->len ,  val ,  arr ->element_size * size ) ;
 
 arr ->len  +=  size ;
 
 
 }
 array array_reverse(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * arr .element_size ,  & /*vvar*/  ( *(void**) array__get( a , a .len - 1 - i) ) ,  arr .element_size ) ;
 
 }
 ;
 
return  arr ;
 
 
 }
 array array_clone(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 memcpy ( arr .data ,  a .data ,  a .cap * a .element_size ) ;
 
return  arr ;
 
 
 }
 void v_array_free(array a) {
 
 free ( a .data ) ;
 
 
 }
 string array_string_str(array_string a) {
 
strings__Builder sb= strings__new_builder ( a .len * 3 ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"[") ) ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
string val= ( *(string*) array__get( a , i) ) ;
 
 strings__Builder_write(& /* ? */ sb , _STR("\"%.*s\"", val.len, val.str) ) ;
 
 if ( i < a .len - 1 ) {
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)", ") ) ;
 
 }
 ;
 
 }
 ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"]") ) ;
 
return  strings__Builder_str( sb ) ;
 
 
 }
 string array_byte_hex(array_byte b) {
 
byte* hex= v_malloc ( b .len * 2 + 1 ) ;
 
byte* ptr= & /*vvar*/  hex [/*ptr*/ 0 ]/*rbyte 1*/ ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 
 ptr  +=  sprintf ( ptr ,  "%02x" ,  ( *(byte*) array__get( b , i) ) ) ;
 
 }
 ;
 
return  (tos2((byte *) hex ) ) ;
 
 
 }
 int copy(array_byte dst, array_byte src) {
 
 if ( dst .len > 0  &&  src .len > 0 ) {
 
int min= ( dst .len < src .len ) ? ( dst .len ) : ( src .len ) ;
 
 memcpy ( dst .data ,  array_left( src , min ) .data ,  dst .element_size * min ) ;
 
return  min ;
 
 }
 ;
 
return  0 ;
 
 
 }
 void todo() {
 
 
 }
 string tos(byte* s, int len) {
 
 if ( isnil ( s ) ) {
 
 _panic_debug (31, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/string.v"), tos2((byte *)"builtin"), tos2((byte *)"tos"),  tos2((byte*)"tos(): nil string") ) ;
 
 }
 ;
 
return  (string) { .str =  s , .len =  len } ;
 
 
 }
 string tos_clone(byte* s) {
 
 if ( isnil ( s ) ) {
 
 _panic_debug (41, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/string.v"), tos2((byte *)"builtin"), tos2((byte *)"tos_clone"),  tos2((byte*)"tos: nil string") ) ;
 
 }
 ;
 
return  string_clone( tos2 ( s ) ) ;
 
 
 }
 string tos2(byte* s) {
 
 if ( isnil ( s ) ) {
 
 _panic_debug (50, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/string.v"), tos2((byte *)"builtin"), tos2((byte *)"tos2"),  tos2((byte*)"tos2: nil string") ) ;
 
 }
 ;
 
int len= strlen ( s ) ;
 
string res= tos ( s , len ) ;
 
return  res ;
 
 
 }
 string string_clone(string a) {
 
string b= (string) { .len =  a .len , .str =  v_malloc ( a .len + 1 ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 b .str[ i ]/*rbyte 1*/  =  a .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
 b .str[ a .len ]/*rbyte 1*/  =  '\0' ;
 
return  b ;
 
 
 }
 string string_replace(string s, string rep, string with) {
 
 if ( s .len == 0  ||  rep .len == 0 ) {
 
return  s ;
 
 }
 ;
 
array_int idxs=new_array_from_c_array(0, 0, sizeof(int), (int[]) {   0 }) ;
 
 {
 
 }
 
string rem= s ;
 
int rstart= 0 ;
 
 while (1) { 
 
int i= string_index( rem , rep ) ;
 
 if ( i < 0 ) {
 
 break
 ;
 
 }
 ;
 
_PUSH(& idxs , ( rstart + i ), tmp9, int) ;
 
 i  +=  rep .len ;
 
 rstart  +=  i ;
 
 rem  =  string_substr( rem , i , rem .len ) ;
 
 }
 ;
 
 if ( idxs .len == 0 ) {
 
return  s ;
 
 }
 ;
 
int new_len= s .len + idxs .len * ( with .len - rep .len ) ;
 
byte* b= v_malloc ( new_len + 1 ) ;
 
int idx_pos= 0 ;
 
int cur_idx= ( *(int*) array__get( idxs , idx_pos) ) ;
 
int b_i= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( i == cur_idx ) {
 
 for (
int j= 0  ;  j < with .len  ;  j ++ ) { 
 
 
 b [/*ptr*/ b_i ]/*rbyte 1*/  =  with .str[ j ]/*rbyte 0*/ ;
 
 b_i ++ ;
 
 }
 ;
 
 i  +=  rep .len - 1 ;
 
 idx_pos ++ ;
 
 if ( idx_pos < idxs .len ) {
 
 cur_idx  =  ( *(int*) array__get( idxs , idx_pos) ) ;
 
 }
 ;
 
 }
  else { 
 
 b [/*ptr*/ b_i ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 
 b_i ++ ;
 
 }
 ;
 
 }
 ;
 
 b [/*ptr*/ new_len ]/*rbyte 1*/  =  '\0' ;
 
return  tos ( b , new_len ) ;
 
 
 }
 int v_string_int(string s) {
 
return  atoi ( s .str ) ;
 
 
 }
 i32 string_i32(string s) {
 
return  atol ( s .str ) ;
 
 
 }
 i64 string_i64(string s) {
 
return  atoll ( s .str ) ;
 
 
 }
 f32 string_f32(string s) {
 
return  atof ( s .str ) ;
 
 
 }
 f64 string_f64(string s) {
 
return  atof ( s .str ) ;
 
 
 }
 u32 string_u32(string s) {
 
return  strtoul ( s .str ,  0 ,  0 ) ;
 
 
 }
 u64 string_u64(string s) {
 
return  strtoull ( s .str ,  0 ,  0 ) ;
 
 
 }
 bool string_eq(string s, string a) {
 
 if ( isnil ( s .str ) ) {
 
 _panic_debug (161, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/string.v"), tos2((byte *)"builtin"), tos2((byte *)"eq"),  tos2((byte*)"string.eq(): nil string") ) ;
 
 }
 ;
 
 if ( s .len != a .len ) {
 
return  0 ;
 
 }
 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( s .str[ i ]/*rbyte 0*/ != a .str[ i ]/*rbyte 0*/ ) {
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 
 }
 bool string_ne(string s, string a) {
 
return  ! string_eq( s , a ) ;
 
 
 }
 bool string_lt(string s, string a) {
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( i >= a .len  ||  s .str[ i ]/*rbyte 0*/ > a .str[ i ]/*rbyte 0*/ ) {
 
return  0 ;
 
 }
  else  if ( s .str[ i ]/*rbyte 0*/ < a .str[ i ]/*rbyte 0*/ ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
 if ( s .len < a .len ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 bool string_le(string s, string a) {
 
return  string_lt( s , a )  ||  string_eq( s , a ) ;
 
 
 }
 bool string_gt(string s, string a) {
 
return  ! string_le( s , a ) ;
 
 
 }
 bool string_ge(string s, string a) {
 
return  ! string_lt( s , a ) ;
 
 
 }
 string string_add(string s, string a) {
 
int new_len= a .len + s .len ;
 
string res= (string) { .len =  new_len , .str =  v_malloc ( new_len + 1 ) } ;
 
 for (
int j= 0  ;  j < s .len  ;  j ++ ) { 
 
 
 res .str[ j ]/*rbyte 1*/  =  s .str[ j ]/*rbyte 0*/ ;
 
 }
 ;
 
 for (
int j= 0  ;  j < a .len  ;  j ++ ) { 
 
 
 res .str[ s .len + j ]/*rbyte 1*/  =  a .str[ j ]/*rbyte 0*/ ;
 
 }
 ;
 
 res .str[ new_len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 
 }
 array_string string_split(string s, string delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( delim .len == 0 ) {
 
_PUSH(& res , ( s ), tmp28, string) ;
 
return  res ;
 
 }
 ;
 
 if ( delim .len == 1 ) {
 
return  string_split_single( s , delim .str[ 0 ]/*rbyte 0*/ ) ;
 
 }
 ;
 
int i= 0 ;
 
int start= 0 ;
 
 while ( i < s .len ) {
 
 
bool a= s .str[ i ]/*rbyte 0*/ == delim .str[ 0 ]/*rbyte 0*/ ;
 
int j= 1 ;
 
 while ( j < delim .len  &&  a ) {
 
 
 a  =  a  &&  s .str[ i + j ]/*rbyte 0*/ == delim .str[ j ]/*rbyte 0*/ ;
 
 j ++ ;
 
 }
 ;
 
bool last= i == s .len - 1 ;
 
 if ( a  ||  last ) {
 
 if ( last ) {
 
 i ++ ;
 
 }
 ;
 
string val= string_substr( s , start , i ) ;
 
 if ( val .len > 0 ) {
 
 if ( string_starts_with( val , delim ) ) {
 
 val  =  string_right( val , delim .len ) ;
 
 }
 ;
 
_PUSH(& res , ( string_trim_space( val ) ), tmp35, string) ;
 
 }
 ;
 
 start  =  i ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  res ;
 
 
 }
 array_string string_split_single(string s, byte delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( ((int)( delim ) ) == 0 ) {
 
_PUSH(& res , ( s ), tmp37, string) ;
 
return  res ;
 
 }
 ;
 
int i= 0 ;
 
int start= 0 ;
 
 while ( i < s .len ) {
 
 
bool is_delim= s .str[ i ]/*rbyte 0*/ == delim ;
 
bool last= i == s .len - 1 ;
 
 if ( is_delim  ||  last ) {
 
 if ( ! is_delim  &&  i == s .len - 1 ) {
 
 i ++ ;
 
 }
 ;
 
string val= string_substr( s , start , i ) ;
 
 if ( val .len > 0 ) {
 
_PUSH(& res , ( val ), tmp43, string) ;
 
 }
 ;
 
 start  =  i + 1 ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  res ;
 
 
 }
 array_string string_split_into_lines(string s) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( s .len == 0 ) {
 
return  res ;
 
 }
 ;
 
int start= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
bool last= i == s .len - 1 ;
 
 if ( ((int)( s .str[ i ]/*rbyte 0*/ ) ) == 10  ||  last ) {
 
 if ( last ) {
 
 i ++ ;
 
 }
 ;
 
string line= string_substr( s , start , i ) ;
 
_PUSH(& res , ( line ), tmp49, string) ;
 
 start  =  i + 1 ;
 
 }
 ;
 
 }
 ;
 
return  res ;
 
 
 }
 string string_left(string s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  string_substr( s , 0 , n ) ;
 
 
 }
 string string_right(string s, int n) {
 
 if ( n >= s .len ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_substr( s , n , s .len ) ;
 
 
 }
 string string_substr(string s, int start, int end) {
 
 if ( start > end  ||  start > s .len  ||  end > s .len  ||  start < 0  ||  end < 0 ) {
 
 _panic_debug (334, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/string.v"), tos2((byte *)"builtin"), tos2((byte *)"substr"),  _STR("substr(%d, %d) out of bounds (len=%d)", start, end, s .len) ) ;
 
 }
 ;
 
int len= end - start ;
 
string res= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 
 res .str [/*ptr*/ i ]/*rbyte 1*/  =  s .str [/*ptr*/ start + i ]/*rbyte 0*/ ;
 
 }
 ;
 
 res .str [/*ptr*/ len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 
 }
 int string_index(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
array_int prefix= array_repeat(& (int[]){  0 },  p .len , sizeof(int) ) ;
 
int j= 0 ;
 
 for (
int i= 1  ;  i < p .len  ;  i ++ ) { 
 
 
 while ( p .str[ j ]/*rbyte 0*/ != p .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 
 j  =  ( *(int*) array__get( prefix , j - 1) ) ;
 
 }
 ;
 
 if ( p .str[ j ]/*rbyte 0*/ == p .str[ i ]/*rbyte 0*/ ) {
 
 j ++ ;
 
 }
 ;
 
array_set(&/*q*/ prefix , i , & (int []) {  j }) ;
 
 }
 ;
 
 j  =  0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 while ( p .str[ j ]/*rbyte 0*/ != s .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 
 j  =  ( *(int*) array__get( prefix , j - 1) ) ;
 
 }
 ;
 
 if ( p .str[ j ]/*rbyte 0*/ == s .str[ i ]/*rbyte 0*/ ) {
 
 j ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
 v_array_free( prefix ) ;
 
return  i - p .len + 1 ;
 
 }
 ;
 
 }
 ;
 
 v_array_free( prefix ) ;
 
return  - 1 ;
 
 
 }
 int string_index_any(string s, string chars) {
 
 string tmp61 =  chars;
 ;
for (int tmp62 = 0; tmp62 < tmp61 .len; tmp62 ++) {
 byte c = ((byte *) tmp61.str)[tmp62];
 
 
int index= string_index( s , byte_str( c ) ) ;
 
 if ( index != - 1 ) {
 
return  index ;
 
 }
 ;
 
 }
 ;
 
return  - 1 ;
 
 
 }
 int string_last_index(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= s .len - p .len ;
 
 while ( i >= 0 ) {
 
 
int j= 0 ;
 
 while ( j < p .len  &&  s .str[ i + j ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i ;
 
 }
 ;
 
 i -- ;
 
 }
 ;
 
return  - 1 ;
 
 
 }
 int string_index_after(string s, string p, int start) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int strt= start ;
 
 if ( start < 0 ) {
 
 strt  =  0 ;
 
 }
 ;
 
 if ( start >= s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= strt ;
 
 while ( i < s .len ) {
 
 
int j= 0 ;
 
int ii= i ;
 
 while ( j < p .len  &&  s .str[ ii ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 ii ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  - 1 ;
 
 
 }
 int string_count(string s, string substr) {
 
 if ( s .len == 0  ||  substr .len == 0 ) {
 
return  0 ;
 
 }
 ;
 
 if ( substr .len > s .len ) {
 
return  0 ;
 
 }
 ;
 
int n= 0 ;
 
int i= 0 ;
 
 while (1) { 
 
 i  =  string_index_after( s , substr , i ) ;
 
 if ( i == - 1 ) {
 
return  n ;
 
 }
 ;
 
 i  +=  substr .len ;
 
 n ++ ;
 
 }
 ;
 
return  0 ;
 
 
 }
 bool string_contains(string s, string p) {
 
bool res= string_index( s , p ) > 0 - 1 ;
 
return  res ;
 
 
 }
 bool string_starts_with(string s, string p) {
 
bool res= string_index( s , p ) == 0 ;
 
return  res ;
 
 
 }
 bool string_ends_with(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  0 ;
 
 }
 ;
 
bool res= string_last_index( s , p ) == s .len - p .len ;
 
return  res ;
 
 
 }
 string string_to_lower(string s) {
 
byte* b= v_malloc ( s .len + 1 ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  tolower ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 
 }
 string string_to_upper(string s) {
 
byte* b= v_malloc ( s .len + 1 ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  toupper ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 
 }
 string string_capitalize(string s) {
 
string sl= string_to_lower( s ) ;
 
string cap=string_add( string_to_upper( byte_str( sl .str[ 0 ]/*rbyte 0*/ ) ) , string_right( sl , 1 ) ) ;
 
return  cap ;
 
 
 }
 string string_title(string s) {
 
array_string words= string_split( s , tos2((byte*)" ") ) ;
 
array_string tit=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 array_string tmp83 =  words;
 ;
for (int tmp84 = 0; tmp84 < tmp83 .len; tmp84 ++) {
 string word = ((string *) tmp83.data)[tmp84];
 
 
_PUSH(& tit , ( string_capitalize( word ) ), tmp85, string) ;
 
 }
 ;
 
string title= array_string_join( tit , tos2((byte*)" ") ) ;
 
return  title ;
 
 
 }
 string string_find_between(string s, string start, string end) {
 
int start_pos= string_index( s , start ) ;
 
 if ( start_pos == - 1 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
string val= string_right( s , start_pos + start .len ) ;
 
int end_pos= string_index( val , end ) ;
 
 if ( end_pos == - 1 ) {
 
return  val ;
 
 }
 ;
 
return  string_left( val , end_pos ) ;
 
 
 }
 bool array_string_contains(array_string ar, string val) {
 
 array_string tmp90 =  ar;
 ;
for (int tmp91 = 0; tmp91 < tmp90 .len; tmp91 ++) {
 string s = ((string *) tmp90.data)[tmp91];
 
 
 if (string_eq( s , val ) ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 bool array_int_contains(array_int ar, int val) {
 
 array_int tmp92 =  ar ;
 ;
for (int i = 0; i < tmp92 .len; i ++) {
 int s = ((int *) tmp92 . data)[i];
 
 
 if ( s == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 bool is_space(byte c) {
 
return  isspace ( c ) ;
 
 
 }
 bool byte_is_space(byte c) {
 
return  is_space ( c ) ;
 
 
 }
 string string_trim_space(string s) {
 
 if (string_eq( s , tos2((byte*)"") ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int i= 0 ;
 
 while ( i < s .len  &&  is_space ( s .str[ i ]/*rbyte 0*/ ) ) {
 
 
 i ++ ;
 
 }
 ;
 
int end= s .len - 1 ;
 
 while ( end >= 0  &&  is_space ( s .str[ end ]/*rbyte 0*/ ) ) {
 
 
 end -- ;
 
 }
 ;
 
 if ( i > end + 1 ) {
 
return  s ;
 
 }
 ;
 
string res= string_substr( s , i , end + 1 ) ;
 
return  res ;
 
 
 }
 string string_trim(string s, string cutset) {
 
 if ( s .len == 0  ||  cutset .len == 0 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos_left= 0 ;
 
int pos_right= s .len - 1 ;
 
bool cs_match= 1 ;
 
 while ( pos_left <= s .len  &&  pos_right >= - 1  &&  cs_match ) {
 
 
 cs_match  =  0 ;
 
 if (_IN(byte,  s .str[ pos_left ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 pos_left ++ ;
 
 cs_match  =  1 ;
 
 }
 ;
 
 if (_IN(byte,  s .str[ pos_right ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 pos_right -- ;
 
 cs_match  =  1 ;
 
 }
 ;
 
 if ( pos_left > pos_right ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
 }
 ;
 
return  string_substr( s , pos_left , pos_right + 1 ) ;
 
 
 }
 string string_trim_left(string s, string cutset) {
 
 if ( s .len == 0  ||  cutset .len == 0 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos= 0 ;
 
 while ( pos <= s .len  && _IN(byte,  s .str[ pos ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 
 pos ++ ;
 
 }
 ;
 
return  string_right( s , pos ) ;
 
 
 }
 string string_trim_right(string s, string cutset) {
 
 if ( s .len == 0  ||  cutset .len == 0 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos= s .len - 1 ;
 
 while ( pos >= - 1  && _IN(byte,  s .str[ pos ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 
 pos -- ;
 
 }
 ;
 
return  string_left( s , pos + 1 ) ;
 
 
 }
 int compare_strings(string* a, string* b) {
 
 if ( string_lt(* a ,* b ) ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( string_gt(* a ,* b ) ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 int compare_strings_by_len(string* a, string* b) {
 
 if ( a ->len < b ->len ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( a ->len > b ->len ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 int compare_lower_strings(string* a, string* b) {
 
string aa= string_to_lower(* a ) ;
 
string bb= string_to_lower(* b ) ;
 
return  compare_strings (& /*112 EXP:"string*" GOT:"string" */ aa ,& /*112 EXP:"string*" GOT:"string" */ bb ) ;
 
 
 }
 void array_string_sort(array_string* s) {
 
 array_sort_with_compare( s , compare_strings ) ;
 
 
 }
 void array_string_sort_ignore_case(array_string* s) {
 
 array_sort_with_compare( s , compare_lower_strings ) ;
 
 
 }
 void array_string_sort_by_len(array_string* s) {
 
 array_sort_with_compare( s , compare_strings_by_len ) ;
 
 
 }
 ustring string_ustring(string s) {
 
ustring res= (ustring) { .s =  s , .runes =  new_array ( 0 , s .len , sizeof( int) ) , .len = 0 } ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
_PUSH(& res .runes , ( i ), tmp109, int) ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 
 }
 ustring string_ustring_tmp(string s) {
 
 if ( g_ustring_runes .len == 0 ) {
 
 g_ustring_runes  =  new_array ( 0 , 128 , sizeof( int) ) ;
 
 }
 ;
 
ustring res= (ustring) { .s =  s , .runes = new_array(0, 1, sizeof( int )) , .len = 0 } ;
 
 res .runes  =  g_ustring_runes ;
 
 res .runes .len  =  s .len ;
 
int j= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
array_set(&/*q*/ res .runes , j , & (int []) {  i }) ;
 
 j ++ ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 
 }
 string ustring_substr(ustring u, int _start, int _end) {
 
int start= ( *(int*) array__get( u .runes , _start) ) ;
 
int end= ( _end >= u .runes .len ) ? ( u .s .len ) : ( ( *(int*) array__get( u .runes , _end) ) ) ;
 
return  string_substr( u .s , start , end ) ;
 
 
 }
 string ustring_left(ustring u, int pos) {
 
return  ustring_substr( u , 0 , pos ) ;
 
 
 }
 string ustring_right(ustring u, int pos) {
 
return  ustring_substr( u , pos , u .len ) ;
 
 
 }
 byte string_at(string s, int idx) {
 
 if ( idx < 0  ||  idx >= s .len ) {
 
 _panic_debug (743, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/string.v"), tos2((byte *)"builtin"), tos2((byte *)"at"),  _STR("string index out of range: %d / %d", idx, s .len) ) ;
 
 }
 ;
 
return  s .str [/*ptr*/ idx ]/*rbyte 0*/ ;
 
 
 }
 string ustring_at(ustring u, int idx) {
 
return  ustring_substr( u , idx , idx + 1 ) ;
 
 
 }
 void v_ustring_free(ustring u) {
 
 v_array_free( u .runes ) ;
 
 
 }
 bool byte_is_digit(byte c) {
 
return  c >= '0'  &&  c <= '9' ;
 
 
 }
 bool byte_is_hex_digit(byte c) {
 
return  byte_is_digit( c )  ||  ( c >= 'a'  &&  c <= 'f' )  ||  ( c >= 'A'  &&  c <= 'F' ) ;
 
 
 }
 bool byte_is_oct_digit(byte c) {
 
return  c >= '0'  &&  c <= '7' ;
 
 
 }
 bool byte_is_letter(byte c) {
 
return  ( c >= 'a'  &&  c <= 'z' )  ||  ( c >= 'A'  &&  c <= 'Z' ) ;
 
 
 }
 void v_string_free(string s) {
 
 v_free ( s .str ) ;
 
 
 }
 string string_all_before(string s, string dot) {
 
int pos= string_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 
 }
 string string_all_before_last(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 
 }
 string string_all_after(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_right( s , pos + dot .len ) ;
 
 
 }
 string array_string_join(array_string a, string del) {
 
 if ( a .len == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int len= 0 ;
 
 array_string tmp124 =  a ;
 ;
for (int i = 0; i < tmp124 .len; i ++) {
 string val = ((string *) tmp124 . data)[i];
 
 
 len  +=  val .len + del .len ;
 
 }
 ;
 
 len  -=  del .len ;
 
string res= tos2((byte*)"") ;
 
 res .len  =  len ;
 
 res .str  =  v_malloc ( res .len + 1 ) ;
 
int idx= 0 ;
 
 array_string tmp127 =  a ;
 ;
for (int i = 0; i < tmp127 .len; i ++) {
 string val = ((string *) tmp127 . data)[i];
 
 
 for (
int j= 0  ;  j < val .len  ;  j ++ ) { 
 
 
byte c= val .str[ j ]/*rbyte 0*/ ;
 
 res .str [/*ptr*/ idx ]/*rbyte 1*/  =  val .str [/*ptr*/ j ]/*rbyte 0*/ ;
 
 idx ++ ;
 
 }
 ;
 
 if ( i != a .len - 1 ) {
 
 for (
int k= 0  ;  k < del .len  ;  k ++ ) { 
 
 
 res .str [/*ptr*/ idx ]/*rbyte 1*/  =  del .str [/*ptr*/ k ]/*rbyte 0*/ ;
 
 idx ++ ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
 res .str [/*ptr*/ res .len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 
 }
 string array_string_join_lines(array_string s) {
 
return  array_string_join( s , tos2((byte*)"\n") ) ;
 
 
 }
 string string_reverse(string s) {
 
string res= (string) { .len =  s .len , .str =  v_malloc ( s .len ) } ;
 
 for (
int i= s .len - 1  ;  i >= 0  ;  i -- ) { 
 
 
 res .str[ s .len - i - 1 ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
return  res ;
 
 
 }
 string string_limit(string s, int max) {
 
ustring u= string_ustring( s ) ;
 
 if ( u .len <= max ) {
 
return  s ;
 
 }
 ;
 
return  ustring_substr( u , 0 , max ) ;
 
 
 }
 bool byte_is_white(byte c) {
 
int i= ((int)( c ) ) ;
 
return  i == 10  ||  i == 32  ||  i == 9  ||  i == 13  ||  c == '\r' ;
 
 
 }
 int string_hash(string s) {
 
int h= 0 ;
 
 if ( h == 0  &&  s .len > 0 ) {
 
 string tmp136 =  s;
 ;
for (int tmp137 = 0; tmp137 < tmp136 .len; tmp137 ++) {
 byte c = ((byte *) tmp136.str)[tmp137];
 
 
 h  =  h * 31 + ((int)( c ) ) ;
 
 }
 ;
 
 }
 ;
 
return  h ;
 
 
 }
 array_byte string_bytes(string s) {
 
 if ( s .len == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(byte), (byte[]) {   0 }) ;
 
 }
 ;
 
array_byte buf= array_repeat(& (byte[]){  ((byte)( 0 ) ) },  s .len , sizeof(byte) ) ;
 
 memcpy ( buf .data ,  s .str ,  s .len ) ;
 
return  buf ;
 
 
 }
 void v_exit(int code) {
 
 exit ( code ) ;
 
 
 }
 bool isnil(void* v) {
 
return  v == 0 ;
 
 
 }
 void on_panic(int (*f)( int  /*FFF*/ )) {
 
 
 }
 void print_backtrace_skipping_top_frames(int skipframes) {
 
 #ifdef __APPLE__
 
byte* buffer  [100 ]= {0} ;
 
void* nr_ptrs= backtrace ( buffer ,  100 ) ;
 
 backtrace_symbols_fd ( & /*vvar*/  buffer [ skipframes ]/*rbyte* 0*/ , (byte*) nr_ptrs - skipframes ,  1 ) ;
 
 return ;
 
 #endif
 ;
 
 #ifdef __linux__
 
 if ( backtrace_symbols_fd != 0 ) {
 
byte* buffer  [100 ]= {0} ;
 
void* nr_ptrs= backtrace ( buffer ,  100 ) ;
 
 backtrace_symbols_fd ( & /*vvar*/  buffer [ skipframes ]/*rbyte* 0*/ , (byte*) nr_ptrs - skipframes ,  1 ) ;
 
 return ;
 
 }
  else { 
 
 printf ( "backtrace_symbols_fd is missing, so printing backtraces is not available.\n" ) ;
 
 printf ( "Some libc implementations like musl simply do not provide it.\n" ) ;
 
 }
 ;
 
 #endif
 ;
 
 printf ( "print_backtrace_skipping_top_frames is not implemented on this platform for now...\n" ) ;
 
 
 }
 void print_backtrace() {
 
 print_backtrace_skipping_top_frames ( 2 ) ;
 
 
 }
 void _panic_debug(int line_no, string file, string mod, string fn_name, string s) {
 
 println ( tos2((byte*)"================ V panic ================") ) ;
 
printf( "   module: %.*s\n", mod.len, mod.str ) ;
 
printf( " function: %.*s()\n", fn_name.len, fn_name.str ) ;
 
printf( "     file: %.*s\n", file.len, file.str ) ;
 
 println (string_add( tos2((byte*)"     line: ") , int_str( line_no ) ) ) ;
 
printf( "  message: %.*s\n", s.len, s.str ) ;
 
 println ( tos2((byte*)"=========================================") ) ;
 
 print_backtrace_skipping_top_frames ( 1 ) ;
 
 exit ( 1 ) ;
 
 
 }
 void v_panic(string s) {
 
printf( "V panic: %.*s\n", s.len, s.str ) ;
 
 print_backtrace ( ) ;
 
 exit ( 1 ) ;
 
 
 }
 void println(string s) {
 
 if ( isnil ( s .str ) ) {
 
 _panic_debug (70, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/builtin.v"), tos2((byte *)"builtin"), tos2((byte *)"println"),  tos2((byte*)"println(NIL)") ) ;
 
 }
 ;
 
 #ifdef _WIN32
 
 _putws ( string_to_wide( s ) ) ;
 
 #else
 
 printf ( "%.*s\n" ,  s .len ,  s .str ) ;
 
 #endif
 ;
 
 
 }
 void eprintln(string s) {
 
 if ( isnil ( s .str ) ) {
 
 _panic_debug (81, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/builtin.v"), tos2((byte *)"builtin"), tos2((byte *)"eprintln"),  tos2((byte*)"eprintln(NIL)") ) ;
 
 }
 ;
 
 #ifdef __APPLE__
 
 fprintf ( stderr ,  "%.*s\n" ,  s .len ,  s .str ) ;
 
 #else
 
 println ( s ) ;
 
 #endif
 ;
 
 
 }
 void print(string s) {
 
 #ifdef _WIN32
 
 wprintf ( string_to_wide( s ) ) ;
 
 #else
 
 printf ( "%.*s" ,  s .len ,  s .str ) ;
 
 #endif
 ;
 
 
 }
 byte* v_malloc(int n) {
 
 if ( n < 0 ) {
 
 _panic_debug (104, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/builtin.v"), tos2((byte *)"builtin"), tos2((byte *)"malloc"),  tos2((byte*)"malloc(<0)") ) ;
 
 }
 ;
 
byte* ptr= malloc ( n ) ;
 
 if ( isnil ( ptr ) ) {
 
 _panic_debug (122, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/builtin.v"), tos2((byte *)"builtin"), tos2((byte *)"malloc"),  _STR("malloc(%d) failed", n) ) ;
 
 }
 ;
 
return  ptr ;
 
 
 }
 byte* v_calloc(int n) {
 
 if ( n < 0 ) {
 
 _panic_debug (129, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/builtin.v"), tos2((byte *)"builtin"), tos2((byte *)"calloc"),  tos2((byte*)"calloc(<0)") ) ;
 
 }
 ;
 
return  calloc ( n ,  1 ) ;
 
 
 }
 void v_free(void* ptr) {
 
 free ( ptr ) ;
 
 
 }
 void* memdup(void* src, int sz) {
 
byte* mem= v_malloc ( sz ) ;
 
return  memcpy ( mem ,  src ,  sz ) ;
 
 
 }
 string f64_str(f64 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( buf ,  "%f" ,  d ) ;
 
return  tos ( buf , strlen ( buf ) ) ;
 
 
 }
 string f32_str(f32 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( buf ,  "%f" ,  d ) ;
 
return  tos ( buf , strlen ( buf ) ) ;
 
 
 }
 string ptr_str(void* ptr) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( buf ,  "%p" ,  ptr ) ;
 
return  tos ( buf , strlen ( buf ) ) ;
 
 
 }
 bool f64_eq(f64 a, f64 b) {
 
return  ( a - b ) <= DBL_EPSILON ;
 
 
 }
 string int_str(int nn) {
 
int n= nn ;
 
 if ( n == 0 ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 16 ;
 
byte* buf= v_calloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < 0 ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > 0 ) {
 
 
int d= n % 10 ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / 10 ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 string u32_str(u32 nn) {
 
u32 n= nn ;
 
 if ( n == ((u32)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 16 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
 while ( n > ((u32)( 0 ) ) ) {
 
 
u32 d= n % ((u32)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u32)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u32)( 10 ) ) ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 string u8_str(u8 nn) {
 
u8 n= nn ;
 
 if ( n == ((u8)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 5 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
 while ( n > ((u8)( 0 ) ) ) {
 
 
u8 d= n % ((u8)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u8)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u8)( 10 ) ) ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 string i64_str(i64 nn) {
 
i64 n= nn ;
 
 if ( n == ((i64)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 32 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < ((i64)( 0 ) ) ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > ((i64)( 0 ) ) ) {
 
 
int d= ((int)( n % ((i64)( 10 ) ) ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((i64)( 10 ) ) ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 string u64_str(u64 nn) {
 
u64 n= nn ;
 
 if ( n == ((u64)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 32 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
 while ( n > ((u64)( 0 ) ) ) {
 
 
u64 d= n % ((u64)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u64)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u64)( 10 ) ) ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 
 }
 string bool_str(bool b) {
 
 if ( b ) {
 
return  tos2((byte*)"true") ;
 
 }
 ;
 
return  tos2((byte*)"false") ;
 
 
 }
 string int_hex(int n) {
 
int len= ( n >= 0 ) ? ( int_str( n ) .len + 3 ) : ( 11 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( hex ,  "0x%x" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 
 }
 string i64_hex(i64 n) {
 
int len= ( n >= ((i64)( 0 ) ) ) ? ( i64_str( n ) .len + 3 ) : ( 19 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( hex ,  "0x%x" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 
 }
 bool array_byte_contains(array_byte a, byte val) {
 
 array_byte tmp37 =  a;
 ;
for (int tmp38 = 0; tmp38 < tmp37 .len; tmp38 ++) {
 byte aa = ((byte *) tmp37.data)[tmp38];
 
 
 if ( aa == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 
 }
 string rune_str(rune c) {
 
int fst_byte= ((int)( c ) )  >>  8 * 3 & 0xff ;
 
int len= utf8_char_len ( fst_byte ) ;
 
string str= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 
 str .str [/*ptr*/ i ]/*rbyte 1*/  =  ((int)( c ) )  >>  8 * ( 3 - i ) & 0xff ;
 
 }
 ;
 
 str .str[ len ]/*rbyte 1*/  =  '\0' ;
 
return  str ;
 
 
 }
 string byte_str(byte c) {
 
string str= (string) { .len =  1 , .str =  v_malloc ( 2 ) } ;
 
 str .str [/*ptr*/ 0 ]/*rbyte 1*/  =  c ;
 
 str .str [/*ptr*/ 1 ]/*rbyte 1*/  =  '\0' ;
 
return  str ;
 
 
 }
 bool byte_is_capital(byte c) {
 
return  c >= 'A'  &&  c <= 'Z' ;
 
 
 }
 array_byte array_byte_clone(array_byte b) {
 
array_byte res= array_repeat(& (byte[]){  ((byte)( 0 ) ) },  b .len , sizeof(byte) ) ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 
array_set(&/*q*/ res , i , & (byte []) {  ( *(byte*) array__get( b , i) ) }) ;
 
 }
 ;
 
return  res ;
 
 
 }
 int utf8_char_len(byte b) {
 
return  ( ( 0xe5000000  >>  ( ( b  >>  3 ) & 0x1e ) ) & 3 ) + 1 ;
 
 
 }
 string utf32_to_str(u32 code) {
 
int icode= ((int)( code ) ) ;
 
byte* buffer= v_malloc ( 5 ) ;
 
 if ( icode <= 127 ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  icode ;
 
return  tos ( buffer , 1 ) ;
 
 }
 ;
 
 if ( ( icode <= 2047 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  192 | ( icode  >>  6 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 2 ) ;
 
 }
 ;
 
 if ( ( icode <= 65535 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  224 | ( icode  >>  12 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 3 ) ;
 
 }
 ;
 
 if ( ( icode <= 1114111 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  240 | ( icode  >>  18 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 3 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 4 ) ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 
 }
 string utf32_to_str_no_malloc(u32 code, void* buf) {
 
int icode= ((int)( code ) ) ;
 
byteptr buffer= ((byteptr)( buf ) ) ;
 
 if ( icode <= 127 ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  icode ;
 
return  tos ( buffer , 1 ) ;
 
 }
 ;
 
 if ( ( icode <= 2047 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  192 | ( icode  >>  6 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 2 ) ;
 
 }
 ;
 
 if ( ( icode <= 65535 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  224 | ( icode  >>  12 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 3 ) ;
 
 }
 ;
 
 if ( ( icode <= 1114111 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  240 | ( icode  >>  18 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 3 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 4 ) ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 
 }
 int string_utf32_code(string _rune) {
 
 if ( _rune .len == 0 ) {
 
return  0 ;
 
 }
 ;
 
 if ( _rune .len == 1 ) {
 
return  ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ;
 
 }
 ;
 
byte b= ((byte)( ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ) ) ;
 
 b  =  b  <<  _rune .len ;
 
int res= ((int)( b ) ) ;
 
int shift= 6 - _rune .len ;
 
 for (
int i= 1  ;  i < _rune .len  ;  i ++ ) { 
 
 
int c= ((int)( _rune .str[ i ]/*rbyte 0*/ ) ) ;
 
 res  =  res  <<  shift ;
 
 res  |=  c & 63 ;
 
 shift  =  6 ;
 
 }
 ;
 
return  res ;
 
 
 }
 u16* string_to_wide(string _str) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( MultiByteToWideChar ( builtin__CP_UTF8 ,  0 ,  _str .str ,  _str .len ,  0 ,  0 ) ) ) ;
 
u16* wstr= ((u16*)( v_malloc ( ( num_chars + 1 ) * 2 ) ) ) ;
 
 if ( wstr > 0 ) {
 
 MultiByteToWideChar ( builtin__CP_UTF8 ,  0 ,  _str .str ,  _str .len ,  wstr ,  num_chars ) ;
 
 memset ( ((byte*)( wstr ) ) + num_chars * 2 ,  0 ,  2 ) ;
 
 }
 ;
 
return  wstr ;
 
 #else
 
return  0 ;
 
 #endif
 ;
 
 
 }
 string string_from_wide(u16* _wstr) {
 
 #ifdef _WIN32
 
int wstr_len= ((int)( wcslen ( _wstr ) ) ) ;
 
return  string_from_wide2 ( _wstr , wstr_len ) ;
 
 #else
 
return  tos2((byte*)"") ;
 
 #endif
 ;
 
 
 }
 string string_from_wide2(u16* _wstr, int len) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len ,  0 ,  0 ,  0 ,  0 ) ) ) ;
 
byte* str_to= ((byte*)( v_malloc ( num_chars + 1 ) ) ) ;
 
 if ( str_to > 0 ) {
 
 WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len ,  str_to ,  num_chars ,  0 ,  0 ) ;
 
 memset ( ((byte*)( str_to ) ) + num_chars ,  0 ,  1 ) ;
 
 }
 ;
 
return  tos2 ( str_to ) ;
 
 #else
 
return  tos2((byte*)"") ;
 
 #endif
 ;
 
 
 }
 map new_map(int cap, int elm_size) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size = 0 } ;
 
return  res ;
 
 
 }
 map new_map_init(int cap, int elm_size, string* keys, void* vals) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size = 0 } ;
 
 int tmp3 =  0;
 ;
for (int tmp4 = tmp3; tmp4 <  cap; tmp4++) {
 int i = tmp4;
 
 
 map__set(& /* ? */ res , keys [/*ptr*/ i ]/*rstring 0*/ ,(byte*) vals + i * elm_size ) ;
 
 }
 ;
 
return  res ;
 
 
 }
 mapnode* new_node(string key, void* val, int element_size) {
 
mapnode* new_e= (mapnode*)memdup(&(mapnode)  { .key =  key , .val =  v_malloc ( element_size ) , .left =  0 , .right =  0 , .is_empty = 0 , } , sizeof(mapnode)) ;
 
 memcpy ( new_e ->val ,  val ,  element_size ) ;
 
return  new_e ;
 
 
 }
 void map_insert(map* m, mapnode* n, string key, void* val) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( n ->val ,  val ,  m ->element_size ) ;
 
 return ;
 
 }
 ;
 
 if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
 n ->left  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 }
  else { 
 
 map_insert( m , n ->left , key , val ) ;
 
 }
 ;
 
 return ;
 
 }
 ;
 
 if ( isnil ( n ->right ) ) {
 
 n ->right  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 }
  else { 
 
 map_insert( m , n ->right , key , val ) ;
 
 }
 ;
 
 
 }
 bool mapnode_find(mapnode* n, string key, void* out, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( out ,  n ->val ,  element_size ) ;
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find(& /* ? */* n ->left , key , out , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find(& /* ? */* n ->right , key , out , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 
 }
 bool mapnode_find2(mapnode* n, string key, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find2(& /* ? */* n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find2(& /* ? */* n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 
 }
 void map__set(map* m, string key, void* val) {
 
 if ( isnil ( m ->root ) ) {
 
 m ->root  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 return ;
 
 }
 ;
 
 map_insert( m , m ->root , key , val ) ;
 
 
 }
 int preorder_keys(mapnode* node, array_string* keys, int key_i) {
 
int i= key_i ;
 
 if ( ! node ->is_empty ) {
 
array_string a= * keys ;
 
array_set(&/*q*/ a , i , & (string []) {  node ->key }) ;
 
 i ++ ;
 
 }
 ;
 
 if ( ! isnil ( node ->left ) ) {
 
 i  =  preorder_keys ( node ->left , keys , i ) ;
 
 }
 ;
 
 if ( ! isnil ( node ->right ) ) {
 
 i  =  preorder_keys ( node ->right , keys , i ) ;
 
 }
 ;
 
return  i ;
 
 
 }
 array_string map_keys(map* m) {
 
array_string keys= array_repeat(& (string[]){  tos2((byte*)"") },  m ->size , sizeof(string) ) ;
 
 if ( isnil ( m ->root ) ) {
 
return  keys ;
 
 }
 ;
 
 preorder_keys ( m ->root ,& /*111*/ (array[]){ keys }[0]  , 0 ) ;
 
return  keys ;
 
 
 }
 bool map_get(map m, string key, void* out) {
 
 if ( isnil ( m .root ) ) {
 
return  0 ;
 
 }
 ;
 
return  mapnode_find(& /* ? */* m .root , key , out , m .element_size ) ;
 
 
 }
 void mapnode_delete(mapnode* n, string key, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
 memset ( n ->val ,  0 ,  element_size ) ;
 
 n ->is_empty  =  1 ;
 
 return ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
 return ;
 
 }
  else { 
 
 mapnode_delete( n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
 return ;
 
 }
  else { 
 
 mapnode_delete( n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 
 }
 void map_delete(map* m, string key) {
 
 mapnode_delete( m ->root , key , m ->element_size ) ;
 
 m ->size -- ;
 
 
 }
 bool map_exists(map m, string key) {
 
 _panic_debug (214, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/map.v"), tos2((byte *)"builtin"), tos2((byte *)"exists"),  tos2((byte*)"map.exists(key) was removed from the language. Use `key in map` instead.") ) ;
 
 
 }
 bool map__exists(map m, string key) {
 
return  ! isnil ( m .root )  &&  mapnode_find2(& /* ? */* m .root , key , m .element_size ) ;
 
 
 }
 void map_print(map m) {
 
 println ( tos2((byte*)"<<<<<<<<") ) ;
 
 println ( tos2((byte*)">>>>>>>>>>") ) ;
 
 
 }
 void v_map_free(map m) {
 
 
 }
 string map_string_str(map_string m) {
 
 if ( m .size == 0 ) {
 
return  tos2((byte*)"{}") ;
 
 }
 ;
 
strings__Builder sb= strings__new_builder ( 50 ) ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"{") ) ;
 
 map_string tmp10 =  m ;
 array_string keys_tmp10 = map_keys(& tmp10 ); 
 for (int l = 0; l < keys_tmp10 .len; l++) {
   string key = ((string*)keys_tmp10 .data)[l];
 string val = {0}; map_get(tmp10, key, & val);
 
 
 strings__Builder_writeln(& /* ? */ sb , _STR("  \"%.*s\" => \"%.*s\"", key.len, key.str, val.len, val.str) ) ;
 
 }
 ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"}") ) ;
 
return  strings__Builder_str( sb ) ;
 
 
 }
 Option opt_ok(void* data, int size) {
 
 if ( size >= 255 ) {
 
 _panic_debug (15, tos2((byte *)"/home/rakhan/zbin/v/vlib/builtin/option.v"), tos2((byte *)"builtin"), tos2((byte *)"opt_ok"),  _STR("option size too big: %d (max is 255), this is a temporary limit", size) ) ;
 
 }
 ;
 
Option res= (Option) { .ok =  1 , .error = tos((byte *)"", 0) , } ;
 
 memcpy ( res .data ,  data ,  size ) ;
 
return  res ;
 
 
 }
 Option v_error(string s) {
 
return  (Option) { .error =  s , .ok = 0 } ;
 
 
 }
 strings__Builder strings__new_builder(int initial_size) {
 
return  (strings__Builder) { .buf =  _make ( 0 , initial_size , sizeof( byte) ) , .len = 0 } ;
 
 
 }
 void strings__Builder_write(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
 b ->len  +=  s .len ;
 
 
 }
 void strings__Builder_writeln(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
_PUSH(& b ->buf , ( '\n' ), tmp1, byte) ;
 
 b ->len  +=  s .len + 1 ;
 
 
 }
 string strings__Builder_str(strings__Builder b) {
 
return  (tos((byte *) b .buf .data ,  b .len ) ) ;
 
 
 }
 void strings__Builder_cut(strings__Builder* b, int n) {
 
 b ->len  -=  n ;
 
 
 }
 void strings__Builder_free(strings__Builder* b) {
 
 v_free ( b ->buf .data ) ;
 
 
 }
 string strings__repeat(byte c, int n) {
 
 if ( n <= 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
byte* arr= v_malloc ( n + 1 ) ;
 
 for (
int i= 0  ;  i < n  ;  i ++ ) { 
 
 
 arr [/*ptr*/ i ]/*rbyte 1*/  =  c ;
 
 }
 ;
 
 arr [/*ptr*/ n ]/*rbyte 1*/  =  '\0' ;
 
return  (tos((byte *) arr ,  n ) ) ;
 
 
 }
 int main(int argc, char** argv) {
 init_consts();
 
int n= 3 ;
 
f32 a= 1.0 ;
 
