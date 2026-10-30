# Module 01 디펜스 대비 노트

> 평가 전 이 문서부터 읽을 것. "왜 이렇게 짰는지"를 설명할 수 있어야 함.
> Module 00 노트는 `../00/DEFENSE_NOTES.md` 참고 (데이터 중심 사고, 캡슐화/추상화 판단 기준 등 공통 프레임워크는 거기 정리되어 있음).

---

## 이 모듈의 핵심 주제

- 메모리 할당 (스택 vs 힙)
- 멤버 함수 포인터
- 참조 (References)
- `switch`문

## ex00 — BraiiiiiinnnzzzZ

### 값(value) 중심 → 정체성(identity) 중심으로의 전환
- 이전(Module 00)까지는 "데이터=값"이었음. 복사되든 참조되든 내용만 같으면 됐고, 언제 사라지는지는 안 중요했음
- `Zombie`는 **정체성을 가진 객체**임. 복사본을 만들면 "같은 이름의 다른 좀비"가 되는 것이지 "원래 그 좀비"가 아님 → 그래서 "이 객체가 정확히 언제 태어나고 죽는가"를 처음으로 명시적으로 추적해야 함 (생성자/소멸자)
- 이건 OOP만의 문제가 아니라, C의 `malloc`/`free`에서도 동일하게 존재하는 "자원 생명주기 관리" 문제. 클래스의 생성자/소멸자는 이걸 더 안전하게 다루는 도구일 뿐

### `newZombie`/`randomChump`는 왜 `Zombie`의 멤버가 아니라 자유 함수인가
- 스펙의 "Files to Submit"이 `Zombie.cpp`와 `newZombie.cpp`/`randomChump.cpp`를 **별도 파일로 분리**해서 요구함 → 멤버 함수였다면 `Zombie.cpp` 안에 같이 두는 게 자연스러웠을 것
- 함수 시그니처도 `Zombie::` 스코프 지정 없이 그냥 `Zombie* newZombie(...)` 형태로 적혀 있음
- 개념적으로도: 이 두 함수는 "좀비 객체 자체의 행동"이 아니라 "좀비를 스택에 만들지 힙에 만들지 결정하는 바깥의 정책"에 가까움. `Zombie` 클래스는 자기가 어디서 만들어지는지 알 필요가 없음

### 스택 vs 힙 판단 기준
- **`newZombie`**: 만든 좀비를 **함수 스코프 밖으로 반환**해서 계속 써야 함 → 힙(`new`)에 만들고 포인터로 반환. 프로그래머가 `delete`할 때까지 생존
- **`randomChump`**: 함수 안에서 `announce()`까지 다 끝내고 버려짐 → 스택에 만들어도 충분, 함수 끝나면 자동 소멸
- 판단 기준: **"이 객체가 함수 스코프보다 오래 살아야 하는가?"** — 그렇다면 힙, 아니면 스택

### 왜 `Zombie`를 값으로 반환하면 (포인터 대신) 문제가 생기나
- 값으로 반환해도 크래시는 안 남 (복사해서 반환하니 댕글링 아님) — 하지만:
  - `return z;` 하는 순간, 함수가 끝나면서 **원본 `z`의 소멸자가 바로 호출**됨 → "소멸됨" 디버그 메시지가 찍히는데, 호출한 쪽은 방금 받은 **복사본**을 여전히 멀쩡히 쓰고 있는 모순된 상황이 됨
  - 그래서 "디버깅용 소멸 메시지"가 의미를 가지려면, 진짜로 그 객체가 죽는 시점(`delete` 호출 시점)에 정확히 소멸자가 불려야 하고, 이건 힙+포인터 방식이라야 보장됨

### 진짜 위험한 경우 (지금 코드와는 다름, 구분해서 알아둘 것)
```cpp
Zombie* bad(std::string name)
{
	Zombie z(name);   // 스택
	return &z;         // ❌ 스택 변수의 주소를 반환 — 함수 끝나면 댕글링 포인터
}
```
`new`로 힙에 만든 걸 반환하는 것(안전)과, 스택 변수의 주소를 반환하는 것(위험)은 다른 이야기. 헷갈리지 말 것.

### `.` vs `->`
- `zombie`가 포인터(`Zombie *`)면 `zombie->announce()` (포인터는 먼저 역참조해야 멤버 접근 가능)
- `zombie`가 실제 객체(`Zombie`)면 `zombie.announce()`
- `p->method()`는 `(*p).method()`의 축약형. 같은 포인터로 메서드를 여러 번 부를 거면 `Zombie &z = *p;`로 한 번만 참조로 풀어두고 `.`으로 쭉 쓰는 것도 방법 (단, `delete`는 반드시 원래 포인터로)

### 확인 완료
- `-Wall -Wextra -Werror`, `-std=c++98` 컴파일 클린
- `leaks` 도구로 메모리 누수 0건 확인
- 실행 결과로 스택/힙 소멸 타이밍 차이 실제로 관찰됨 (`randomChump`는 매 호출마다 즉시 소멸 메시지, `newZombie`는 `delete` 시점에만 소멸 메시지)

## ex01 — Moar brainz!

### 왜 기본 생성자(`Zombie()`)가 필요했나
- `new Zombie[N]`(배열 `new`)은 **각 원소를 기본 생성자로 초기화**함. 인자를 받는 생성자만 있으면 배열 생성 자체가 컴파일 안 됨
- 그래서 "일단 이름 없이 만들고" → `setName()`으로 "나중에 이름을 채워 넣는" 2단계 구조가 필요해짐 (Module 00의 `Contact`가 기본 생성자 + `setContact()`로 채워 넣던 패턴과 동일한 이유)

### 왜 `setName()`이 `public`이어야 했나
- `zombieHorde()`는 `Zombie`의 멤버 함수가 아니라 **자유 함수**임 (ex00에서 확인한 것과 같은 이유 — 파일이 분리되어 있고, 시그니처에 `Zombie::`가 없음)
- 자유 함수는 클래스의 `private` 멤버에 접근할 권한이 없음. `horde[i].setName(name)`을 자유 함수 안에서 호출하려면 `setName`이 `public`이어야 함
- (반대로 `announce()`처럼 클래스 스스로 하는 행동은 계속 `public`에 있어도 됨 — 어차피 원래도 public이었음)

### 반복해서 걸렸던 패턴: `Type::` 스코프 누락
- `void setName(...)`처럼 `Zombie::`를 안 붙이면, 이건 `Zombie`와 무관한 **독립 함수**로 컴파일됨 → 그 안에서 `_name`(private 멤버)에 접근하려 해도 "그런 변수 없음" 에러가 남
- Module 00에서부터 계속 반복된 실수 패턴 — `.cpp`에서 멤버 함수 구현할 땐 항상 `ReturnType ClassName::methodName(...)` 형태인지 확인할 것

### `.cpp`를 `#include`하면 안 되는 이유 (실전 사례)
- `ZombieHorde.cpp`에서 `#include "Zombie.cpp"`를 했었는데, `Makefile`이 `Zombie.cpp`를 **이미 따로** 컴파일하고 있어서, 같은 함수 정의가 두 번(각각 `Zombie.o`와 `ZombieHorde.o`에) 들어가게 되어 **링크 단계에서 "중복 심볼" 에러**가 남
- 헤더(`.hpp`)는 include해도 안전하지만(include guard가 중복 정의를 막아줌), `.cpp`는 include guard 유무와 상관없이 "실행 가능한 코드"를 통째로 복사하는 거라 원칙적으로 include 대상이 아님

### `delete` vs `delete[]`
- `new Zombie[N]`으로 만든 건 반드시 `delete[] horde;`로 해제해야 함 (`delete horde;`로 하면 첫 번째 원소만 소멸되고 나머지는 누수되거나, 최악의 경우 UB)
- **할당 형태와 해제 형태를 짝 맞추는 게 규칙**: `new`↔`delete`, `new[]`↔`delete[]`

### 포인터 하나로 배열 전체 순회하기
- `zombieHorde`가 반환하는 건 "첫 번째 좀비를 가리키는 포인터"일 뿐, 그 자체로는 "내가 몇 개짜리 배열의 일부인지" 전혀 모름 (타입 정보에 크기가 안 담김)
- `horde[i]`(= `*(horde + i)`)로 접근 가능한 이유는 `new Zombie[N]`이 **연속된 메모리**를 보장해주기 때문. 따로따로 `new`를 N번 하면 메모리가 이어져 있다는 보장이 없어서 이 방식 자체가 성립 안 함
- 그래서 호출하는 쪽(`main`)이 `N`을 별도로 기억해서 반복문을 돌려야 함 — 포인터는 크기 정보를 안 갖고 있으므로

### Makefile 실행 파일 이름 주의
- `NAME`에 공백이나 특수문자(`!` 등)가 들어가면 셸이 인자를 쪼개서 해석함 (`rm -f Moar brainz!`가 "Moar"와 "brainz!" 두 파일을 지우려는 명령으로 해석됨) → 공백/특수문자 없는 이름 사용

### 확인 완료
- `-Wall -Wextra -Werror`, `-std=c++98` 컴파일 클린
- `leaks` 도구로 메모리 누수 0건 확인 (5마리 생성 → 5마리 `delete[]`로 정상 해제)


## ex02 — HI THIS IS BRAIN

### 이 exercise의 핵심
- 포인터와 참조는 **같은 메모리 주소를 가리키는 두 가지 문법**일 뿐, 새로운 마법이 아님 (PDF 원문: "simply another syntax for something you already do: address manipulation")
- 실제로 실행하면 `string`, `stringPTR`, `stringREF` 세 개의 주소가 **전부 동일**하게 출력됨

### 포인터 vs 참조: 값/주소를 얻는 문법이 정반대
| | 값을 얻으려면 | 주소를 얻으려면 |
|---|---|---|
| 포인터 (`stringPTR`) | `*stringPTR` (역참조 필요) | `stringPTR` (변수 자체가 주소) |
| 참조 (`stringREF`) | `stringREF` (원본처럼 그냥 사용) | `&stringREF` (원본의 주소가 나옴) |

### 참조 초기화 문법
- 참조는 **원본 객체 자체**로 초기화함: `std::string &stringREF = string;` (O)
- `= &string;`은 주소(포인터)를 넣는 것이라 타입 불일치 에러 (X)
- 이유: 참조는 "주소를 담는 변수"가 아니라 "원본의 또 다른 이름(별칭)"이라서

### `&`의 두 가지 얼굴 (디펜스에서 나올 수 있음)
- **선언 위치**(타입 옆)의 `&` → "참조 타입"이라는 뜻 (`std::string &stringREF`)
- **식 안**(변수 앞)의 `&` → "주소를 구해라"(address-of 연산자) (`&string`, `&stringREF`)
- 기호는 같지만 완전히 다른 문법 요소

### 참조에 `*`를 붙이면 안 되는 이유
- `*stringREF`는 컴파일 에러(`indirection requires pointer operand`). 참조는 이미 원본 자체처럼 동작하므로 역참조가 필요 없음

### 확인 완료
- `-Wall -Wextra -Werror`, `-std=c++98` 컴파일 클린
- 세 주소가 동일하게 출력되고, 세 값도 동일하게 출력됨

## ex03 — Unnecessary violence

### 핵심: HumanA는 참조, HumanB는 포인터인 이유
스펙의 PDF 테스트가 요구하는 동작: `club.setType("...")`로 밖에서 바꾼 내용이 `bob.attack()`/`jim.attack()`에 **반영**돼야 함 → 무기를 **복사해서 들면 안 되고**, 원본을 가리켜야 함.

| | HumanA | HumanB |
|---|---|---|
| 무기 | 생성 시 항상 있음, 평생 같은 무기 | 없을 수도 있음, 나중에 `setWeapon`으로 쥠/교체 |
| 멤버 | `Weapon &_weapon` (참조) | `Weapon *_weapon` (포인터) |
| 이유 | 참조는 비어있을 수 없고 재지정이 안 됨 → "항상 무장, 교체 불가"와 일치 | 포인터는 `NULL`(맨손) 가능, 재지정 가능 → "없을 수도 있고 바뀜"과 일치 |

### 처음 짠 버그: `Weapon _weapon;` (값으로 저장)
- 복사본을 들게 돼서 `club.setType(...)`이 반영 안 됨 (두 번째 `attack()`이 옛날 이름을 출력). PDF 테스트가 정확히 이걸 잡아내려는 것

### 참조 멤버 → 기본 생성자를 만들 수 없음
- 참조는 선언 시점에 반드시 어떤 객체의 별칭이어야 함. 기본 생성자(`HumanA() {}`)는 묶을 `Weapon`이 없어서 컴파일 에러: `constructor must explicitly initialize the reference member`
- 참조 멤버는 **생성자 초기화 리스트**에서만 묶을 수 있음 (본문에서 대입하면 이미 늦음): `: _name(name), _weapon(weapon)`
- 이건 "초기값 없이는 만들 수 없다"를 언어 차원에서 강제하는 것 (Module 00 `Account`의 private 기본 생성자와 같은 발상)
- 이번엔 배열/`new[]`가 없어서 기본 생성자가 필요 없음 (Module 00 `Contact`, ex01 `Zombie`가 필요했던 건 배열 때문)

### 넣을 때와 쓸 때
| | 넣을 때 | 쓸 때 |
|---|---|---|
| 참조 | 원본 그대로 (`_weapon(weapon)`) | `.` (`_weapon.getType()`) |
| 포인터 | 주소로 (`_weapon = &weapon;`) | `->` (`_weapon->getType()`) |
- `setWeapon(Weapon &weapon)`의 `weapon`은 참조라서 `&weapon`하면 **참조 자신이 아니라 원본의 주소**가 나옴 → 그걸 포인터 멤버에 저장

### 포인터는 반드시 NULL로 초기화
- `Weapon *_weapon;`을 초기화 안 하면 **쓰레기 주소**가 들어있음. `setWeapon` 전에 `attack()`하면 쓰레기를 따라가서 크래시
- `: _weapon(NULL)`로 초기화하고, `attack()`에서 `if (_weapon)`으로 분기. 무기 없으면 `attacks with their hands` 출력 (스펙에 없어서 직접 정한 "relevant behavior")
- `_weapon("hand")`처럼 문자열을 넣으려던 시도는 타입 불일치. "맨손"은 값이 아니라 `NULL`이라는 **상태**로 표현
- C++98이라 `nullptr`은 못 씀 (C++11)

### const의 세 가지 위치
```cpp
const std::string  getName() const;   // ① 값 반환 앞 const: 복사본이라 거의 무의미
const std::string &getName() const;   // ② 참조 반환 앞 const: 원본을 읽기 전용으로 노출 (의미 있음)
                                       // ③ 뒤쪽 const: 이 함수는 멤버를 안 바꾼다는 약속 (의미 있음)
```
- 스펙이 `getType()`을 "constant reference 반환"으로 명시 → `const std::string &getType() const;`
- `std::string &getName() const`(비-const 참조 + const 함수)는 **컴파일 에러**: const 함수 안에서 `_name`은 const라서 const 없는 참조로 내보낼 수 없음 (const 우회 방지)

### 수명(댕글링) 문제
- C++는 `Weapon`이 `Human`보다 먼저 사라지는 걸 **막아주지 않음** (참조든 포인터든 똑같이 위험)
- 안전하게 쓰려면 **`Weapon`을 `Human`보다 먼저, 같은 블록에 선언** → 지역 변수는 **선언의 역순**으로 소멸되니 `Human`이 먼저 죽고 `Weapon`이 나중에 죽음
- 복사해서 갖거나 스마트 포인터를 쓰면 해결되지만, 복사는 이 exercise의 핵심을 깨고 스마트 포인터는 C++11이라 금지
- `HumanB`는 무기를 **빌려 쓸 뿐 소유하지 않음** → `new`가 없으니 `delete`도 없음

### 참조 vs 포인터 배경 (디펜스에서 나올 수 있음)
- 포인터는 C부터 있던 "주소" 도구, 참조는 C++가 추가한 "별칭" 도구. 참조의 핵심 동기는 **연산자 오버로딩**(`a + b`를 주소 문법 없이, 복사 없이 쓰기 위해)
- 참조: 선언 시 초기화 필수, null 불가, 재지정 불가, `&ref`는 원본 주소. `r = b;`는 "b를 가리켜라"가 아니라 **원본에 값 대입**
- 복사 생성자가 `Foo(const Foo &)`여야 하는 이유: 값으로 받으면 인자를 넘기려고 또 복사 생성자가 호출돼 무한 재귀 (Module 02부터 나옴)
- 기본은 참조, 참조로 안 되는 상황(없을 수 있음 / 교체 / 힙 `new`)에서만 포인터
- 구현은 컴파일러마다 다르지만(지역 참조는 최적화로 메모리 없을 수도, 멤버 참조는 보통 포인터 크기만큼 차지), 언어 규칙상 참조는 별칭

### 확인 완료
- `-Wall -Wextra -Werror`, `-std=c++98` 컴파일 클린
- PDF 테스트 두 케이스 모두 `crude spiked club` → `some other type of club` 반영
- 무기 없는 `HumanB::attack()`도 크래시 없이 동작
- `leaks`로 메모리 누수 0건

## ex04 — Sed is for losers

### 왜 `Replacer` 클래스를 만들었나 (그리고 왜 `File` 클래스는 아닌가)
- 스펙은 클래스를 요구하지 않음. 클래스는 **속도/효율이 아니라 "상태 유지 + 규칙 보호"**가 필요할 때 쓰는 것 (Module 00 판단 기준)
- `File` 같은 래퍼는 불필요: `std::ifstream`/`std::ofstream`이 **이미 파일 클래스**임. 감싸봐야 `open()`을 그대로 전달만 하는 껍데기(과설계, Megaphone을 클래스로 감싸는 것과 같음). `ifstream`을 상속하는 것도 말이 안 됨 ("is-a"가 아님)
- `Replacer`는 `(filename, s1, s2)` **세 값을 한 묶음으로 들고, 그 묶음의 규칙(`s1`은 비면 안 됨)을 책임**지므로 정당화 가능 (선택 사항, 안 만들어도 감점 없음)
- 구조: `replaceAll`(private 순수 함수: 데이터 in → 데이터 out)과 `run`(파일 I/O)을 분리 — `toUpper`/`toUpperStr` 때와 같은 "계산과 I/O 분리"
- 기본 생성자 없음: 세 값 없이는 의미가 없고 배열/`new[]`도 없으므로
- 생성자는 실패를 반환할 수 없고(예외는 아직 안 배움) → 검증은 `run()`에서 하고 `bool` 반환, `main`이 종료 코드 결정

### `ifstream` / `ofstream` (`<fstream>`)
- `cout`/`cin`/`ostringstream`과 같은 스트림 계열, 목적지만 파일. `<<`, `>>`, `std::getline`을 그대로 씀
- **C++98에서는 파일명에 `.c_str()` 필요** (`std::string` 직접 받는 생성자는 C++11부터). `ofstream`에도 동일 (`(_filename + ".replace").c_str()`)
- 소멸자가 자동으로 파일을 닫음 (자원 = 객체 생명주기, ex00 `Zombie`와 같은 원리)
- 열기는 실패할 수 있음 → `is_open()` 확인 필수. **출력 파일도 확인**해야 함 (권한/경로 문제)

### 파일 전체 읽기: `getline` + 개행 복원
```cpp
while (std::getline(in, line)) { content += line; if (!in.eof()) content += '\n'; }
```
- `getline`은 개행을 **읽고 버림** → 그냥 이어붙이면 한 줄로 합쳐지고, 매번 `\n`을 붙이면 마지막 줄 뒤에 없던 개행이 생김
- 마지막 `getline` 후 `eof()`: 끝에 개행이 **있으면 false**(개행에서 멈춤), **없으면 true**(파일 끝까지 읽음) → `!eof()`일 때만 개행 복원하면 원본과 바이트 단위로 동일

### `replaceAll` (`std::string::replace`만 금지, `find`/`substr`/`+=`는 허용)
```cpp
while ((found = content.find(_s1, pos)) != std::string::npos)
{ result += content.substr(pos, found - pos); result += _s2; pos = found + _s1.length(); }
result += content.substr(pos);
```
- **원본 `content`에서만 찾고 결과는 별도 `result`에 쌓음** → `s2` 안에 `s1`이 있어도(`a`→`aa`) 무한 증식 안 함
- `pos`를 `found + _s1.length()`로 건너뛰어 치환된 `s2`를 다시 검색하지 않음
- `s1`이 **빈 문자열**이면 `find`가 매번 같은 자리에서 찾아 무한 루프 → `run()`에서 반드시 거부
- 겹침(`aaa`에서 `aa`)은 왼쪽부터 겹치지 않게: `ba` (python `str.replace`와 같은 의미)

### `std::string::size_type`
- 위치/길이를 나타내는 **부호 없는 정수 타입**. `find`가 못 찾으면 `std::string::npos`(= `size_type(-1)`, 부호 없는 최댓값)를 반환
- `int`로 받으면 `npos` 비교가 꼬이고 `-Wextra`의 sign-compare가 `-Werror`로 에러. `size_t`도 보통 같은 타입이지만 표준이 보장하진 않아서 `std::string::size_type`이 정확

### 테스트가 찾아낸 진짜 버그: 입력이 디렉터리일 때
- 디렉터리도 `ifstream`이 열기는 성공(`is_open()` 참), 읽기만 조용히 실패 → **빈 파일로 취급해 종료 코드 0으로 성공**하고 빈 `.replace` 생성 (macOS libc++에서 `eof`/`fail`/`bad` 상태가 빈 파일과 똑같아서 `bad()`로도 못 잡음)
- 해결: `seekg`/`tellg`로 **파일 크기를 구하고, 실제 읽은 길이와 다르면 오류**로 처리 (`size > 0`일 때만: 파이프처럼 크기를 알 수 없는 입력은 건너뜀). 정상 파일은 복원한 내용이 크기와 항상 같음

### 테스트 구성 (`ex04/tests/`) — 총 32개 통과
- `input/`, `expected/`(파일 20개씩), `test.sh`, `README.md`. 실행: `make` 후 `bash tests/test.sh`
- 임시 폴더에서 실행해 원본 입력을 안 건드리고, `cmp`로 **바이트 단위** 비교
- 에러 케이스는 "종료 코드 ≠ 0"만이 아니라 **종료 코드 == 1, 출력 메시지가 정확히 일치, 출력 파일이 안 남음** 세 가지를 확인하고, 실패하면 예상값/실제값을 나란히 출력 (엉뚱한 이유로 실패해도 OK가 뜨는 걸 방지)
- 테스트가 실제로 틀린 걸 잡는지 **일부러 코드를 망가뜨려서 확인**: 메시지를 바꾸면 메시지 불일치로, 디렉터리 검사를 빼면 종료 코드/메시지/출력 파일 세 가지 모두로 FAIL이 뜸
- `make test` 규칙은 **일부러 안 넣음**: Makefile은 필수 제출 파일이고 표준 규칙(`all`/`clean`/`fclean`/`re`)만 두는 게 안전
- 테스트 파일 제출 근거: General Rules "추가 파일은 필수 파일만 제대로 내면 자유" + ex04 스펙의 "must create and turn in your own tests"
- 추가로 **파이썬 `str.replace`를 정답으로 쓰는 무작위 비교 1500건 전부 일치**, 10MB(50만 줄) 0.17초, 5MB 한 줄/500만 치환 정상 (임시 확인용, 제출물 아님)
- 확인한 특수 입력: CRLF, UTF-8(한글), NUL 바이트, 이진 바이트, 공백/`-` 시작 파일명, 심볼릭 링크, 이미 있는 `.replace` 덮어쓰기, 읽기 전용 출력 파일

### 확인 완료
- `-Wall -Wextra -Werror`, `-std=c++98` 컴파일 클린, `leaks` 0건

## ex05 — Harl 2.0

### 요구사항의 핵심
- private `debug`/`info`/`warning`/`error`, public `complain(std::string level)`가 레벨 문자열에 맞는 함수를 호출
- **`if`/`else if` 나열 금지, 멤버 함수 포인터로 구현**이 필수 ("This is not a suggestion"). 이 모듈 제목의 "pointers to members"가 여기서 실전으로 나옴
- 문구는 자유 (PDF 예시는 참고용). 모르는 레벨일 때 동작은 스펙에 없어서 직접 정함 → 아무것도 출력하지 않음

### `complain`의 구조: 이름 배열 + 함수 포인터 배열을 같은 번호로 짝짓기
```cpp
const std::string levels[4] = { "DEBUG", "INFO", "WARNING", "ERROR" };
void (Harl::*functions[4])(void) = { &Harl::debug, &Harl::info, &Harl::warning, &Harl::error };

for (int i = 0; i < 4; i++)
    if (level == levels[i]) { (this->*functions[i])(); return ; }
```
- 이름이 `levels[i]`와 같으면 `functions[i]`를 호출. 함수마다 `if`를 쓰는 "숲" 대신 **데이터(배열)로 분기를 표현**
- 레벨이 늘어나면 두 배열에 한 칸씩만 추가하면 됨 (확장이 쉬움). 루프 안의 `if` 하나는 "이름 찾기"일 뿐 함수별 분기가 아님

### 멤버 함수 포인터 문법
| 표현 | 뜻 |
|---|---|
| `void (Harl::*ptr)(void)` | "`Harl`의 멤버 함수 중, 인자 없고 `void`를 반환하는 것을 가리키는 포인터 `ptr`" (타입에 **클래스 이름**이 들어감) |
| `&Harl::debug` | `Harl`의 `debug` 멤버 함수의 "주소"(클래스 안에서의 위치). 반드시 `&`와 `Harl::`을 붙임 |
| `(obj.*ptr)()` / `(objPtr->*ptr)()` | **어떤 객체에 대해** `ptr`이 가리키는 함수를 호출 (`.*`는 객체, `->*`는 객체 포인터) |
| `(this->*functions[i])()` | `complain`을 부른 **바로 그 객체(`this`)**에 대해 `i`번째 함수를 호출 |

- 일반 함수 포인터와 다른 점: 멤버 함수는 **객체 없이는 호출할 수 없어서** (`this`가 필요) 호출할 때 객체를 같이 넘겨야 함
- 호출식 전체를 **괄호로 감싸야** 함: `(this->*functions[i])()`. `.*`/`->*`의 우선순위가 함수 호출 `()`보다 낮아서, 괄호가 없으면 `this->*(functions[i]())`로 읽혀 에러가 남
- private 함수인데 포인터로 호출 가능한 이유: 포인터를 만든 곳(`complain`)이 **`Harl`의 멤버 함수 안**이라 private 접근 권한이 있음. 밖에서는 `&Harl::debug`를 취할 수 없음

### 왜 `if` 숲보다 좋은가 (디펜스에서 나올 수 있음)
- 분기 로직이 코드가 아니라 **표(배열)**로 바뀜 → 새 레벨 추가 = 표에 한 줄. `if` 숲은 레벨마다 분기 코드를 새로 짜야 함
- 레벨 이름과 함수의 대응 관계가 한눈에 보임
- 단점도 인지: 배열 두 개의 **순서가 어긋나면** 엉뚱한 함수가 호출됨 → 같은 줄에 나란히 적어 대응을 눈으로 확인 (구조체 배열로 묶는 방법도 있지만 이번엔 과함)

### ex03과의 연결
- ex03은 "참조 vs 포인터"로 **데이터**를 가리키는 법을 배웠고, ex05는 같은 포인터 개념을 **함수(행동)**에 적용한 것. "무엇을 할지"를 변수처럼 저장하고 고를 수 있음

### 확인 완료
- `-Wall -Wextra -Werror`, `-std=c++98` 컴파일 클린, `leaks` 0건
- 4개 레벨 출력 확인, 모르는 레벨(`PANIC`, 빈 문자열, 소문자 `debug`)은 아무것도 출력 안 함

## ex06 — Harl filter (선택: "이 exercise 없이도 모듈 통과")

### 요구사항의 핵심
- 레벨 하나를 인자로 받아 **그 레벨과 그 위 레벨 전부** 출력 (`WARNING` → `WARNING`, `ERROR`). 레벨 순서: `DEBUG` < `INFO` < `WARNING` < `ERROR`
- 실행 파일 이름 `harlFilter`, **`switch`문 사용 필수**
- 모르는 문자열 → `[ Probably complaining about insignificant problems ]` 한 줄. 인자 개수 틀림은 스펙에 없어서 직접 정함 (`Usage: ./harlFilter <level>`, 종료 코드 1)
- 출력 형식(PDF 예시): 각 레벨이 `[ LEVEL ]` 줄로 시작하고 메시지 뒤에 빈 줄. 모르는 레벨 한 줄 뒤에는 빈 줄 없음

### ex05와의 차이: 분기 기법이 다름
| | ex05 | ex06 |
|---|---|---|
| 분기 | 멤버 함수 포인터 배열 | `switch` |
| 동작 | 레벨 **하나**만 불평 | 지정 레벨 **이상 전부** 출력 |
| 함수 | 메시지만 출력 | 각 함수가 **자기 `[ LEVEL ]` 헤더까지** 출력 |

### 구조
```cpp
int index = 4;                                     // 4 = 모르는 레벨
for (int i = 0; i < 4; i++)
    if (level == levels[i]) { index = i; break ; }

switch (index)
{
    case 0: this->debug();
    case 1: this->info();
    case 2: this->warning();
    case 3: this->error(); break ;
    default: std::cout << "[ Probably complaining about insignificant problems ]" << std::endl;
}
```

### 왜 문자열을 숫자로 먼저 바꾸나 (`switch (level)`이 안 되는 이유)
- `switch`는 **정수 계열**(`int`, `char`, `enum`)만 받고, `case` 라벨도 컴파일 시점의 **정수 상수**여야 함. `std::string`은 둘 다 불가 → 컴파일 에러 (`statement requires expression of integer type`)
- Module 00 PhoneBook의 `ADD`/`SEARCH`/`EXIT`를 `if`/`else if`로 처리한 것과 같은 이유
- `level[0]`(첫 글자)로 `switch`하는 꼼수는 `DOG`, `Error` 같은 **첫 글자만 맞는 입력**이 걸려서 부정확 → 이름 → 인덱스 변환이 정확

### fall-through (일부러 `break`를 뺀 이유)
- `break`가 없으면 아래 `case`로 계속 흘러내림. 보통은 버그지만 여기서는 **"이 레벨 이상 전부"**와 정확히 일치: `WARNING`(2)으로 들어가면 `warning()` → `error()` 순으로 실행
- 디펜스 답변: "이 레벨 이상을 전부 출력하려고 `break`를 일부러 뺐다"
- **GCC 주의 (직접 확인함):** GCC는 `-Wextra`에 `-Wimplicit-fallthrough`가 포함돼서, `break` 없는 `case` 사이에 `// fall through` 주석이 없으면 `error: this statement may fall through [-Werror=implicit-fallthrough=]`로 **빌드 실패** (g++-15로 확인). 주석을 넣으면 통과. clang은 기본 `-Wextra`에서 이 경고를 안 켜서 주석 없이도 통과. C++98에는 `[[fallthrough]]`가 없어서 주석으로 표기. 이 맥의 `g++`는 clang 별칭이라 진짜 GCC는 `g++-15`(Homebrew)로 확인해야 함. 또 GCC 경고는 `-fsyntax-only`로는 안 나오고 실제 컴파일(`-c`)에서만 나옴. **현재 코드는 `case` 사이에 `// fall through`를 넣어둔 상태 (GCC 15, clang 모두 통과 확인). 설명용이 아니라 GCC를 통과시키는 기능성 주석이라 빼면 안 됨** (norm 검사가 없는 C++ 모듈이라 주석을 써도 문제없음)

### 확인 완료
- `DEBUG`→4개, `INFO`→3개, `WARNING`→2개, `ERROR`→1개 레벨 출력. 모르는 문자열/소문자 `warning`/빈 문자열 → `[ Probably complaining about insignificant problems ]`
- 인자 없음/2개 → 사용법 출력 + 종료 코드 1
- `-Wall -Wextra -Werror`, `-std=c++98` 컴파일 클린 (clang), `leaks` 0건
