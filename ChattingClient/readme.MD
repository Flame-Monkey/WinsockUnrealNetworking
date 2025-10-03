## ChattingClient 설계

ChattingServer 객체에 login요청, http로 웹서버로부터 로그인 토큰 + 채팅서버 주소 받아옴  
채팅서버에 로그인  

서버객체가 채팅서버와 통신하면서 메시지 조립. main함수 측에서 Get메소드로 메시지 소비

ChattingClient.cpp에서, Send전용 스레드 시작하자마자 queue.empty()가 0을 반환(큐에 뭐가 있대)해서 내부 송신 로직이 돌아가 
에러를 일으킴. 웃긴게 if문 안에서 queue.size()추가하면 이상한 짓 없이 잘 돌아감.. ?????