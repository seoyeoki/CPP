# CPP Module 00 — 규칙 체크리스트 (CPP00.pdf 기준)

> Claude가 앞으로 코드 작성/리뷰 시 이 기준으로 검사함. 위반 시 0점 또는 -42점 처리되는 항목은 **★** 표시.

## 컴파일

- [ ] `c++` 컴파일러 사용, 플래그: `-Wall -Wextra -Werror`
- [ ] `-std=c++98` 플래그를 추가해도 컴파일되어야 함 (C++98 기준으로 작성)

## 네이밍 / 포맷팅

- [ ] exercise 디렉토리명: `ex00`, `ex01`, ... `exn`
- [ ] 클래스명은 **UpperCamelCase**
- [ ] 클래스 파일명은 클래스명과 동일 (`ClassName.hpp`/`.h`, `ClassName.cpp`, `ClassName.tpp`)
- [ ] 별도 지시 없는 한 모든 출력 메시지는 개행(`\n`)으로 끝나야 함
- [ ] Norminette 없음 — 스타일 자유, 단 읽기 쉽게 작성 (동료 평가자가 이해 못 하면 채점 불가)

## 허용 / 금지 ★

- [ ] ★ `using namespace <ns_name>` 금지 (명시적으로 허용된 경우 제외) — 위반 시 **-42점**
- [ ] ★ `friend` 키워드 금지 (명시적으로 허용된 경우 제외) — 위반 시 **-42점**
- [ ] ★ Module 08, 09 이전까지 STL 금지 — Containers(vector/list/map 등) 및 `<algorithm>` 필요한 모든 것 — 위반 시 **-42점**
  - 단, ex02(Job Of Your Dreams)의 `tests.cpp`는 예외로 이미 `<vector>`/`<algorithm>` 사용 (제공된 파일이라 그대로 둠)
- [ ] ★ `*printf()`, `*alloc()`, `free()` 사용 금지 — 위반 시 **grade 0**
- [ ] C++11 이상 문법 및 Boost 라이브러리 금지 (C++98만)

## 설계 요구사항

- [ ] `new`로 할당한 메모리는 반드시 해제 (메모리 누수 방지)
- [ ] Module 02~09: **Orthodox Canonical Form** 준수 (명시적으로 예외 지정된 경우 제외) — *Module 00은 아직 해당 없음, ex02의 Account는 예외적으로 기본 생성자가 private*
- [ ] 헤더 파일에 함수 구현을 넣으면 (함수 템플릿 제외) 해당 exercise 0점
- [ ] 각 헤더는 독립적으로 사용 가능해야 함 (필요한 의존성 모두 include)
- [ ] Include guard 필수 (이중 포함 방지) — 없으면 grade 0

## 제출

- [ ] Git 저장소에 제출한 내용만 평가됨
- [ ] 파일명 정확히 확인

---

# Exercise별 체크

## ex00 — Megaphone
- 제출 파일: `Makefile`, `megaphone.cpp`
- 금지 사항: None
- 요구사항: argv를 대문자로 이어붙여 출력, 인자 없으면 `* LOUD AND UNBEARABLE FEEDBACK NOISE *`

## ex01 — My Awesome PhoneBook
- 제출 파일: `Makefile`, `*.cpp`, `*.{h, hpp}`
- 금지 사항: None
- 요구사항:
  - `PhoneBook` 클래스: 배열로 최대 8개 연락처 저장, 9번째부터는 가장 오래된 것 교체, **동적 할당 금지**
  - `Contact` 클래스: 연락처 1건
  - private/public 구분: 클래스 내부에서만 쓰는 건 private, 외부에서 쓰는 건 public
  - ADD/SEARCH/EXIT 커맨드만 허용, 그 외 입력은 무시
  - ADD: first name, last name, nickname, phone number, darkest secret — 빈 필드 불가
  - SEARCH: index/first name/last name/nickname 4컬럼, 각 10자 폭, `|` 구분자, 우측 정렬, 넘치면 마지막 글자를 `.`로 치환 (iomanip 사용 권장)

## ex02 — The Job Of Your Dreams (필수 아님)
- 제출 파일: `Makefile`, `Account.cpp`, `Account.hpp`, `tests.cpp`
- 금지 사항: None
- 요구사항: 주어진 `Account.hpp` + 로그 파일을 보고 `Account.cpp`를 역추적 구현. 로그의 타임스탬프만 다를 뿐 나머지 출력은 완전히 일치해야 함
- 참고: 소멸자 호출 순서는 컴파일러/OS에 따라 역순일 수 있음 (문제 없음)

---

# Module 01 — Memory allocation, pointers to members, references and switch statements

> General Rules는 Module 00과 동일 (STL 금지 범위, `*printf`/`*alloc`/`free` 금지, `using namespace`/`friend` 금지, include guard 등 전부 동일하게 적용됨)

## ex00 — BraiiiiiinnnzzzZ
- 제출 파일: `Makefile`, `main.cpp`, `Zombie.{h, hpp}`, `Zombie.cpp`, `newZombie.cpp`, `randomChump.cpp`
- 금지 사항: None
- 요구사항:
  - `Zombie` 클래스: private `name`(string), `announce(void)` — `<name>: BraiiiiiinnnzzzZ...` 출력 (꺾쇠는 실제로 안 찍음)
  - `Zombie* newZombie(std::string name);` — 좀비를 만들어서 **포인터로 반환** (함수 스코프 밖에서도 써야 하니 힙 할당 필요)
  - `void randomChump(std::string name);` — 좀비를 만들어서 그 자리에서 `announce()`만 하고 끝 (스코프 밖에서 안 쓰이니 스택 할당 가능)
  - **핵심 질문:** 언제 스택에, 언제 힙에 할당해야 하는지 판단하는 게 이 과제의 목적
  - 소멸자에서 이름과 함께 소멸 메시지 출력 (디버깅용)

## ex01 — Moar brainz!
- 제출 파일: `Makefile`, `main.cpp`, `Zombie.{h, hpp}`, `Zombie.cpp`, `zombieHorde.cpp`
- 금지 사항: None
- 요구사항:
  - `Zombie* zombieHorde(int N, std::string name);` — **단 한 번의 할당**으로 좀비 N마리 생성 (배열 `new`), 전부 같은 이름으로 초기화, 첫 좀비를 가리키는 포인터 반환
  - 직접 테스트 작성해서 각 좀비 `announce()` 확인
  - `delete[]`로 전부 해제하고 **메모리 누수 확인 필수**

## ex02 — HI THIS IS BRAIN
- 제출 파일: `Makefile`, `main.cpp`
- 금지 사항: None
- 요구사항: `'HI THIS IS BRAIN'`로 초기화된 string 변수 + 그 포인터(`stringPTR`) + 그 참조(`stringREF`) 선언. 셋의 **메모리 주소**를 각각 출력한 뒤, 셋의 **값**을 각각 출력. 포인터와 참조의 차이를 체감하는 게 목적

## ex03 — Unnecessary violence
- 제출 파일: `Makefile`, `main.cpp`, `Weapon.{h, hpp}`, `Weapon.cpp`, `HumanA.{h, hpp}`, `HumanA.cpp`, `HumanB.{h, hpp}`, `HumanB.cpp`
- 금지 사항: None
- 요구사항:
  - `Weapon`: private `type`(string), `getType()`(상수 참조 반환), `setType()`
  - `HumanA`/`HumanB`: `Weapon`과 `name`을 가짐, `attack()` — `<name> attacks with their <weapon type>` 출력
  - **`HumanA`는 생성자에서 `Weapon`을 받고 항상 무장 상태** → 참조로 들고 있는 게 적합
  - **`HumanB`는 무기가 없을 수도 있음** → 포인터로 들고 있어야 함 (참조는 "반드시 뭔가를 가리켜야" 해서 null 상태를 표현 못 함)
  - 메모리 누수 확인 필수

## ex04 — Sed is for losers
- 제출 파일: `Makefile`, `main.cpp`, `*.cpp`, `*.{h, hpp}`
- 금지 사항: **`std::string::replace`** ★ (이것만 금지, `std::string`의 다른 멤버 함수는 전부 허용)
- 요구사항: `<filename> <s1> <s2>` 3개 인자, `<filename>` 내용을 복사해서 `<filename>.replace` 생성, 모든 `s1`을 `s2`로 치환. **C 파일 입출력 함수(`fopen` 등) 금지** — 치팅으로 간주됨. 예외 처리 및 자체 테스트 필수

## ex05 — Harl 2.0
- 제출 파일: `Makefile`, `main.cpp`, `Harl.{h, hpp}`, `Harl.cpp`
- 금지 사항: None
- 요구사항:
  - `Harl` 클래스에 private `debug()`, `info()`, `warning()`, `error()` (전부 `void(void)`)
  - public `void complain(std::string level);` — level에 따라 4개 중 하나를 호출
  - **핵심 요구사항: `if`/`else if` 금지, 반드시 멤버 함수 포인터(pointer to member function)로 구현** — 이번 모듈 제목에 나온 "pointers to members"가 여기서 실전으로 쓰임
  - 자체 테스트 작성 필수

## ex06 — Harl filter (필수 아님, "모듈 통과에 ex06 불필요")
- 제출 파일: `Makefile`, `main.cpp`, `Harl.{h, hpp}`, `Harl.cpp`
- 금지 사항: None
- 요구사항: 인자로 받은 레벨 "이상"의 메시지만 출력 (`WARNING` 넘기면 WARNING+ERROR만 출력). 실행 파일명은 `harlFilter`. **`switch`문 사용 필수**

---

# Module 02 — Ad-hoc polymorphism, operator overloading and the Orthodox Canonical class form

> General Rules는 Module 00/01과 동일. **새 규칙: 이제부터 모든 클래스는 Orthodox Canonical Form 필수** (명시적 예외 제외)
> - [ ] Default constructor
> - [ ] Copy constructor
> - [ ] Copy assignment operator
> - [ ] Destructor
> - [ ] 클래스 코드는 두 파일로 분리: 헤더(`.hpp`/`.h`)에 클래스 정의, `.cpp`에 구현
> - 허용 함수: **`roundf` (`<cmath>`)** — ex01 이후에서만 명시적으로 허용됨

## ex00 — My First Class in Orthodox Canonical Form
- 제출 파일: `Makefile`, `main.cpp`, `Fixed.{h, hpp}`, `Fixed.cpp` / 금지: None
- `Fixed` 클래스 (고정소수점 수)
  - private: 값을 저장할 **정수 하나**, **static const int**(소수부 비트 수, 값은 항상 `8`)
  - public: 기본 생성자(값 `0`), 복사 생성자, 복사 대입 연산자, 소멸자, `int getRawBits(void) const;`, `void setRawBits(int const raw);`
- 각 함수가 호출될 때 메시지 출력 (`Default constructor called`, `Copy constructor called`, `Copy assignment operator called`, `Destructor called`, `getRawBits member function called`) — PDF 예시 출력과 비슷해야 함 (복사 대입 메시지 줄은 구현에 따라 없을 수도 있다고 명시)

## ex01 — Towards a more useful fixed-point number class
- 제출 파일: `Makefile`, `main.cpp`, `Fixed.{h, hpp}`, `Fixed.cpp` / 허용: **`roundf` (`<cmath>`)**
- 추가: `const int`를 받는 생성자, `const float`를 받는 생성자 (둘 다 소수부 비트 8로 변환), `float toFloat(void) const;`, `int toInt(void) const;`
- `Fixed.cpp` 쪽에 **`<<` 삽입 연산자 오버로드** (고정소수점 값을 float 표현으로 출력 스트림에 넣음)
- 생성자 메시지: `Int constructor called`, `Float constructor called`

## ex02 — Now we're talking
- 제출 파일: ex01과 동일 / 허용: `roundf`
- 멤버 함수로 연산자 오버로드:
  - 비교 6개: `>`, `<`, `>=`, `<=`, `==`, `!=`
  - 산술 4개: `+`, `-`, `*`, `/`
  - 증감 4개: 전위/후위 `++`, `--` — 값을 표현 가능한 **가장 작은 ε**(`1 + ε > 1`)만큼 증감
- **static 멤버 함수 4개**: `min`(비-const 참조 2개 → 작은 쪽 참조 반환), `min`(**const** 참조 2개), `max`(비-const), `max`(const)
- 0으로 나누기는 프로그램이 크래시해도 허용. 모든 기능을 직접 테스트해야 함

## ex03 — BSP (필수 아님, "모듈 통과에 ex03 불필요")
- 제출 파일: `Makefile`, `main.cpp`, `Fixed.{h, hpp}`, `Fixed.cpp`, `Point.{h, hpp}`, `Point.cpp`, `bsp.cpp` / 허용: `roundf`
- `Point` 클래스 (OCF): private `Fixed const x`, `Fixed const y`; public: 기본 생성자(`0`,`0`), **`const float` 두 개를 받는 생성자**, 복사 생성자, 복사 대입 연산자, 소멸자
- `bool bsp(Point const a, Point const b, Point const c, Point const point);` — `point`가 삼각형 `abc`의 **내부에 있으면 true**. **꼭짓점이나 변 위에 있으면 false**
- 자체 테스트 작성/제출 필수
- ⚠️ `const` 멤버(`x`, `y`)를 가진 클래스의 **복사 대입 연산자**를 어떻게 만들지가 숨은 포인트 (Module 00에서 "const 멤버 + 대입 연산자 충돌" 얘기했던 바로 그 주제)
