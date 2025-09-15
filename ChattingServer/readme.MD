## ChattingServer 설계

main -> ChattingServer init 후 실행  

ChattingServer: 소켓프로그래밍 중심. Accept, Recv이후 Token으로 전달, Token이 요청한 바이트 배열 송신을 처리  

SocketToken: 연결돼있는 소켓 상태 처리.  
데이터를 받으면 MessageManager로 넘긴 후 Message 완성되면 받아서 Server의 MessageHandler로 넘김  
Handler가 토큰에 메시지 송신을 요청하면 바이트 배열로 변환 후 server에 송신 요청을 함  

MessageHandler: 서버 동작 핵심 로직 구현  
각 메시지 핸들러로 전달, DB 조작, 저장해놓은 토큰을 통해 메시지 송신 요청  