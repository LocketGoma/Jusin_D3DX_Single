# Jusin_D3DX_Single
D3DX 개인 포트폴리오 게임 

하프-라이프 2 


[![Video Label](http://img.youtube.com/vi/NMKtPObe68I/0.jpg)](https://youtu.be/NMKtPObe68I)



기간 : 21년 3월~ 21년 5월 (약 2달)

#### 주의사항 : 해당 Repo의 파일만으로는 실행되지 않습니다.
#### 해당 Repo는 코드 백업으로 이해해주시면 감사하겠습니다. 실제 구현 및 플레이를 보시려면 영상 참고 부탁드립니다.

만일 테스트를 위해 빌드를 해보신다면, 아래 정보를 참고해주세요
1. 해당 Repo에는 게임 실행에 필요한 중요 리소스 등이 제거된 상태입니다.
2. Windows 10에서 개발되어, 타 OS에서는 작동을 보장하지 않습니다.

### 빌드시 필요한 사항
1. Visual Studio 2019 이상 설치
2. DirectX 9.0c SDK 설치 [https://www.microsoft.com/en-us/download/details.aspx?id=6812]
3. Windows SDK 8.1 설치 [https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/]
4. VS Installer에서 v142 빌드 도구 설치 (MSVC v142 -VS 2019 C++ x64/x86 빌드 도구) 
5. SDK 8.1 설치 후 나오는 경고창에서 SDK 버전 유지 (10으로 하셔야 합니다)
6. 실행후, Debug / x86 조건으로 실행
7. 정상 실행이 된다면, D3DX_Single_Engine.dll 파일이 없습니다. 라는 메시지와 함께 종료될것입니다.
8. Reference/DLLFiles 에 있는 파일들을 D3DX_Single_GameProject/Binary에 옮기신 뒤, 빌드하시면 됩니다.
9. 실행은 리소스 이슈로 인해 정상적으로 실행되지 않습니다.





### 사용 기술 / 기법 :

DX 9 Framework

네비매시

윈도우 좌표계 변환 마우스 피킹

레이캐스트 기법 활용 메시/버텍스 피킹

구 충돌, AABB 충돌

디졸브 쉐이더


### 주요 구현 :

기본 총기사격

총기 사격/반동

투사체 발사 / 일정 범위내 임의 위치 투사체 발사 (산탄총)

물체 끌어오기 / 튕겨내기 (중력건)

속도 및 질량에 따른 충돌 피해 계산

플레이어 위치 이동에 따른 추적 기능

전용 폰트 활용 UI / 수치 반응형 HUD

브릿지 패턴 활용, 분리되어 조립 가능한 AI 및 게임 오브젝트


### 제작시 활용 코드 기법 / 패턴 :

프로토타입 패턴

컴포넌트 패턴

중재자 패턴

브릿지 패턴
