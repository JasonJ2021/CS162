
a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 7a 2f 00 00    	push   0x2f7a(%rip)        # 3fa0 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 7b 2f 00 00 	bnd jmp *0x2f7b(%rip)        # 3fa8 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	push   $0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	push   $0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    104f:	90                   	nop
    1050:	f3 0f 1e fa          	endbr64 
    1054:	68 02 00 00 00       	push   $0x2
    1059:	f2 e9 c1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    105f:	90                   	nop
    1060:	f3 0f 1e fa          	endbr64 
    1064:	68 03 00 00 00       	push   $0x3
    1069:	f2 e9 b1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    106f:	90                   	nop
    1070:	f3 0f 1e fa          	endbr64 
    1074:	68 04 00 00 00       	push   $0x4
    1079:	f2 e9 a1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    107f:	90                   	nop

Disassembly of section .plt.got:

0000000000001080 <__cxa_finalize@plt>:
    1080:	f3 0f 1e fa          	endbr64 
    1084:	f2 ff 25 6d 2f 00 00 	bnd jmp *0x2f6d(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    108b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

0000000000001090 <setbuf@plt>:
    1090:	f3 0f 1e fa          	endbr64 
    1094:	f2 ff 25 15 2f 00 00 	bnd jmp *0x2f15(%rip)        # 3fb0 <setbuf@GLIBC_2.2.5>
    109b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010a0 <printf@plt>:
    10a0:	f3 0f 1e fa          	endbr64 
    10a4:	f2 ff 25 0d 2f 00 00 	bnd jmp *0x2f0d(%rip)        # 3fb8 <printf@GLIBC_2.2.5>
    10ab:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010b0 <__assert_fail@plt>:
    10b0:	f3 0f 1e fa          	endbr64 
    10b4:	f2 ff 25 05 2f 00 00 	bnd jmp *0x2f05(%rip)        # 3fc0 <__assert_fail@GLIBC_2.2.5>
    10bb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010c0 <pthread_create@plt>:
    10c0:	f3 0f 1e fa          	endbr64 
    10c4:	f2 ff 25 fd 2e 00 00 	bnd jmp *0x2efd(%rip)        # 3fc8 <pthread_create@GLIBC_2.34>
    10cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010d0 <pthread_join@plt>:
    10d0:	f3 0f 1e fa          	endbr64 
    10d4:	f2 ff 25 f5 2e 00 00 	bnd jmp *0x2ef5(%rip)        # 3fd0 <pthread_join@GLIBC_2.34>
    10db:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .text:

00000000000010e0 <_start>:
    10e0:	f3 0f 1e fa          	endbr64 
    10e4:	31 ed                	xor    %ebp,%ebp
    10e6:	49 89 d1             	mov    %rdx,%r9
    10e9:	5e                   	pop    %rsi
    10ea:	48 89 e2             	mov    %rsp,%rdx
    10ed:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    10f1:	50                   	push   %rax
    10f2:	54                   	push   %rsp
    10f3:	45 31 c0             	xor    %r8d,%r8d
    10f6:	31 c9                	xor    %ecx,%ecx
    10f8:	48 8d 3d 58 03 00 00 	lea    0x358(%rip),%rdi        # 1457 <main>
    10ff:	ff 15 d3 2e 00 00    	call   *0x2ed3(%rip)        # 3fd8 <__libc_start_main@GLIBC_2.34>
    1105:	f4                   	hlt    
    1106:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    110d:	00 00 00 

0000000000001110 <deregister_tm_clones>:
    1110:	48 8d 3d 01 2f 00 00 	lea    0x2f01(%rip),%rdi        # 4018 <__TMC_END__>
    1117:	48 8d 05 fa 2e 00 00 	lea    0x2efa(%rip),%rax        # 4018 <__TMC_END__>
    111e:	48 39 f8             	cmp    %rdi,%rax
    1121:	74 15                	je     1138 <deregister_tm_clones+0x28>
    1123:	48 8b 05 b6 2e 00 00 	mov    0x2eb6(%rip),%rax        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    112a:	48 85 c0             	test   %rax,%rax
    112d:	74 09                	je     1138 <deregister_tm_clones+0x28>
    112f:	ff e0                	jmp    *%rax
    1131:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1138:	c3                   	ret    
    1139:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001140 <register_tm_clones>:
    1140:	48 8d 3d d1 2e 00 00 	lea    0x2ed1(%rip),%rdi        # 4018 <__TMC_END__>
    1147:	48 8d 35 ca 2e 00 00 	lea    0x2eca(%rip),%rsi        # 4018 <__TMC_END__>
    114e:	48 29 fe             	sub    %rdi,%rsi
    1151:	48 89 f0             	mov    %rsi,%rax
    1154:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1158:	48 c1 f8 03          	sar    $0x3,%rax
    115c:	48 01 c6             	add    %rax,%rsi
    115f:	48 d1 fe             	sar    %rsi
    1162:	74 14                	je     1178 <register_tm_clones+0x38>
    1164:	48 8b 05 85 2e 00 00 	mov    0x2e85(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable@Base>
    116b:	48 85 c0             	test   %rax,%rax
    116e:	74 08                	je     1178 <register_tm_clones+0x38>
    1170:	ff e0                	jmp    *%rax
    1172:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1178:	c3                   	ret    
    1179:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001180 <__do_global_dtors_aux>:
    1180:	f3 0f 1e fa          	endbr64 
    1184:	80 3d 9d 2e 00 00 00 	cmpb   $0x0,0x2e9d(%rip)        # 4028 <completed.0>
    118b:	75 2b                	jne    11b8 <__do_global_dtors_aux+0x38>
    118d:	55                   	push   %rbp
    118e:	48 83 3d 62 2e 00 00 	cmpq   $0x0,0x2e62(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1195:	00 
    1196:	48 89 e5             	mov    %rsp,%rbp
    1199:	74 0c                	je     11a7 <__do_global_dtors_aux+0x27>
    119b:	48 8b 3d 66 2e 00 00 	mov    0x2e66(%rip),%rdi        # 4008 <__dso_handle>
    11a2:	e8 d9 fe ff ff       	call   1080 <__cxa_finalize@plt>
    11a7:	e8 64 ff ff ff       	call   1110 <deregister_tm_clones>
    11ac:	c6 05 75 2e 00 00 01 	movb   $0x1,0x2e75(%rip)        # 4028 <completed.0>
    11b3:	5d                   	pop    %rbp
    11b4:	c3                   	ret    
    11b5:	0f 1f 00             	nopl   (%rax)
    11b8:	c3                   	ret    
    11b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000011c0 <frame_dummy>:
    11c0:	f3 0f 1e fa          	endbr64 
    11c4:	e9 77 ff ff ff       	jmp    1140 <register_tm_clones>

00000000000011c9 <wrapper>:
  void (*entry)(int);
};

struct thread tpool[NTHREAD], *tptr = tpool;

void *wrapper(void *arg) {
    11c9:	f3 0f 1e fa          	endbr64 
    11cd:	55                   	push   %rbp
    11ce:	48 89 e5             	mov    %rsp,%rbp
    11d1:	48 83 ec 20          	sub    $0x20,%rsp
    11d5:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  struct thread *thread = (struct thread *)arg;
    11d9:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    11dd:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  thread->entry(thread->id);
    11e1:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    11e5:	48 8b 50 10          	mov    0x10(%rax),%rdx
    11e9:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    11ed:	8b 00                	mov    (%rax),%eax
    11ef:	89 c7                	mov    %eax,%edi
    11f1:	ff d2                	call   *%rdx
  return NULL;
    11f3:	b8 00 00 00 00       	mov    $0x0,%eax
}
    11f8:	c9                   	leave  
    11f9:	c3                   	ret    

00000000000011fa <create>:

void create(void *fn) {
    11fa:	f3 0f 1e fa          	endbr64 
    11fe:	55                   	push   %rbp
    11ff:	48 89 e5             	mov    %rsp,%rbp
    1202:	48 83 ec 30          	sub    $0x30,%rsp
    1206:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
  assert(tptr - tpool < NTHREAD);
    120a:	48 8b 05 ff 2d 00 00 	mov    0x2dff(%rip),%rax        # 4010 <tptr>
    1211:	48 8d 15 28 2e 00 00 	lea    0x2e28(%rip),%rdx        # 4040 <tpool>
    1218:	48 29 d0             	sub    %rdx,%rax
    121b:	48 3d e8 05 00 00    	cmp    $0x5e8,%rax
    1221:	7e 28                	jle    124b <create+0x51>
    1223:	48 8d 05 16 0e 00 00 	lea    0xe16(%rip),%rax        # 2040 <__PRETTY_FUNCTION__.0>
    122a:	48 89 c1             	mov    %rax,%rcx
    122d:	ba 1a 00 00 00       	mov    $0x1a,%edx
    1232:	48 8d 05 cb 0d 00 00 	lea    0xdcb(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    1239:	48 89 c6             	mov    %rax,%rsi
    123c:	48 8d 05 ca 0d 00 00 	lea    0xdca(%rip),%rax        # 200d <_IO_stdin_used+0xd>
    1243:	48 89 c7             	mov    %rax,%rdi
    1246:	e8 65 fe ff ff       	call   10b0 <__assert_fail@plt>
  *tptr = (struct thread) {
    124b:	48 8b 05 be 2d 00 00 	mov    0x2dbe(%rip),%rax        # 4010 <tptr>
    .id = tptr - tpool + 1,
    1252:	48 8b 15 b7 2d 00 00 	mov    0x2db7(%rip),%rdx        # 4010 <tptr>
    1259:	48 8d 0d e0 2d 00 00 	lea    0x2de0(%rip),%rcx        # 4040 <tpool>
    1260:	48 29 ca             	sub    %rcx,%rdx
    1263:	48 89 d1             	mov    %rdx,%rcx
    1266:	48 c1 f9 03          	sar    $0x3,%rcx
    126a:	48 ba ab aa aa aa aa 	movabs $0xaaaaaaaaaaaaaaab,%rdx
    1271:	aa aa aa 
    1274:	48 0f af d1          	imul   %rcx,%rdx
    1278:	83 c2 01             	add    $0x1,%edx
    127b:	89 d1                	mov    %edx,%ecx
  *tptr = (struct thread) {
    127d:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
    1281:	48 c7 00 00 00 00 00 	movq   $0x0,(%rax)
    1288:	48 c7 40 08 00 00 00 	movq   $0x0,0x8(%rax)
    128f:	00 
    1290:	48 c7 40 10 00 00 00 	movq   $0x0,0x10(%rax)
    1297:	00 
    1298:	89 08                	mov    %ecx,(%rax)
    129a:	c7 40 04 01 00 00 00 	movl   $0x1,0x4(%rax)
    12a1:	48 89 50 10          	mov    %rdx,0x10(%rax)
    .status = T_LIVE,
    .entry = fn,
  };
  pthread_create(&(tptr->thread), NULL, wrapper, tptr);
    12a5:	48 8b 05 64 2d 00 00 	mov    0x2d64(%rip),%rax        # 4010 <tptr>
    12ac:	48 8b 15 5d 2d 00 00 	mov    0x2d5d(%rip),%rdx        # 4010 <tptr>
    12b3:	48 8d 7a 08          	lea    0x8(%rdx),%rdi
    12b7:	48 89 c1             	mov    %rax,%rcx
    12ba:	48 8d 05 08 ff ff ff 	lea    -0xf8(%rip),%rax        # 11c9 <wrapper>
    12c1:	48 89 c2             	mov    %rax,%rdx
    12c4:	be 00 00 00 00       	mov    $0x0,%esi
    12c9:	e8 f2 fd ff ff       	call   10c0 <pthread_create@plt>
  ++tptr;
    12ce:	48 8b 05 3b 2d 00 00 	mov    0x2d3b(%rip),%rax        # 4010 <tptr>
    12d5:	48 83 c0 18          	add    $0x18,%rax
    12d9:	48 89 05 30 2d 00 00 	mov    %rax,0x2d30(%rip)        # 4010 <tptr>
}
    12e0:	90                   	nop
    12e1:	c9                   	leave  
    12e2:	c3                   	ret    

00000000000012e3 <join>:

void join() {
    12e3:	f3 0f 1e fa          	endbr64 
    12e7:	55                   	push   %rbp
    12e8:	48 89 e5             	mov    %rsp,%rbp
    12eb:	48 83 ec 10          	sub    $0x10,%rsp
  for (int i = 0; i < NTHREAD; i++) {
    12ef:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
    12f6:	eb 51                	jmp    1349 <join+0x66>
    struct thread *t = &tpool[i];
    12f8:	8b 45 f4             	mov    -0xc(%rbp),%eax
    12fb:	48 63 d0             	movslq %eax,%rdx
    12fe:	48 89 d0             	mov    %rdx,%rax
    1301:	48 01 c0             	add    %rax,%rax
    1304:	48 01 d0             	add    %rdx,%rax
    1307:	48 c1 e0 03          	shl    $0x3,%rax
    130b:	48 8d 15 2e 2d 00 00 	lea    0x2d2e(%rip),%rdx        # 4040 <tpool>
    1312:	48 01 d0             	add    %rdx,%rax
    1315:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    if (t->status == T_LIVE) {
    1319:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    131d:	8b 40 04             	mov    0x4(%rax),%eax
    1320:	83 f8 01             	cmp    $0x1,%eax
    1323:	75 20                	jne    1345 <join+0x62>
      pthread_join(t->thread, NULL);
    1325:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1329:	48 8b 40 08          	mov    0x8(%rax),%rax
    132d:	be 00 00 00 00       	mov    $0x0,%esi
    1332:	48 89 c7             	mov    %rax,%rdi
    1335:	e8 96 fd ff ff       	call   10d0 <pthread_join@plt>
      t->status = T_DEAD;
    133a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    133e:	c7 40 04 02 00 00 00 	movl   $0x2,0x4(%rax)
  for (int i = 0; i < NTHREAD; i++) {
    1345:	83 45 f4 01          	addl   $0x1,-0xc(%rbp)
    1349:	83 7d f4 3f          	cmpl   $0x3f,-0xc(%rbp)
    134d:	7e a9                	jle    12f8 <join+0x15>
    }
  }
}
    134f:	90                   	nop
    1350:	90                   	nop
    1351:	c9                   	leave  
    1352:	c3                   	ret    

0000000000001353 <cleanup>:

__attribute__((destructor)) void cleanup() {
    1353:	f3 0f 1e fa          	endbr64 
    1357:	55                   	push   %rbp
    1358:	48 89 e5             	mov    %rsp,%rbp
  join();
    135b:	b8 00 00 00 00       	mov    $0x0,%eax
    1360:	e8 7e ff ff ff       	call   12e3 <join>
}
    1365:	90                   	nop
    1366:	5d                   	pop    %rbp
    1367:	c3                   	ret    

0000000000001368 <set_cur>:

__thread char *base, *cur; // thread-local variables
__thread int id;

// objdump to see how thread-local variables are implemented
__attribute__((noinline)) void set_cur(void *ptr) { cur = ptr; }
    1368:	f3 0f 1e fa          	endbr64 
    136c:	55                   	push   %rbp
    136d:	48 89 e5             	mov    %rsp,%rbp
    1370:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    1374:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    1378:	64 48 89 04 25 f0 ff 	mov    %rax,%fs:0xfffffffffffffff0
    137f:	ff ff 
    1381:	90                   	nop
    1382:	5d                   	pop    %rbp
    1383:	c3                   	ret    

0000000000001384 <get_cur>:
__attribute__((noinline)) char *get_cur()         { return cur; }
    1384:	f3 0f 1e fa          	endbr64 
    1388:	55                   	push   %rbp
    1389:	48 89 e5             	mov    %rsp,%rbp
    138c:	64 48 8b 04 25 f0 ff 	mov    %fs:0xfffffffffffffff0,%rax
    1393:	ff ff 
    1395:	5d                   	pop    %rbp
    1396:	c3                   	ret    

0000000000001397 <stackoverflow>:

void stackoverflow(int n) {
    1397:	f3 0f 1e fa          	endbr64 
    139b:	55                   	push   %rbp
    139c:	48 89 e5             	mov    %rsp,%rbp
    139f:	53                   	push   %rbx
    13a0:	48 83 ec 28          	sub    $0x28,%rsp
    13a4:	89 7d dc             	mov    %edi,-0x24(%rbp)
  set_cur(&n);
    13a7:	48 8d 45 dc          	lea    -0x24(%rbp),%rax
    13ab:	48 89 c7             	mov    %rax,%rdi
    13ae:	e8 b5 ff ff ff       	call   1368 <set_cur>
  if (n % 1024 == 0) {
    13b3:	8b 45 dc             	mov    -0x24(%rbp),%eax
    13b6:	25 ff 03 00 00       	and    $0x3ff,%eax
    13bb:	85 c0                	test   %eax,%eax
    13bd:	75 50                	jne    140f <stackoverflow+0x78>
    int sz = base - get_cur();
    13bf:	64 48 8b 1c 25 e8 ff 	mov    %fs:0xffffffffffffffe8,%rbx
    13c6:	ff ff 
    13c8:	b8 00 00 00 00       	mov    $0x0,%eax
    13cd:	e8 b2 ff ff ff       	call   1384 <get_cur>
    13d2:	48 89 c2             	mov    %rax,%rdx
    13d5:	48 89 d8             	mov    %rbx,%rax
    13d8:	48 29 d0             	sub    %rdx,%rax
    13db:	89 45 ec             	mov    %eax,-0x14(%rbp)
    printf("Stack size of T%d >= %d KB\n", id, sz / 1024);
    13de:	8b 45 ec             	mov    -0x14(%rbp),%eax
    13e1:	8d 90 ff 03 00 00    	lea    0x3ff(%rax),%edx
    13e7:	85 c0                	test   %eax,%eax
    13e9:	0f 48 c2             	cmovs  %edx,%eax
    13ec:	c1 f8 0a             	sar    $0xa,%eax
    13ef:	89 c2                	mov    %eax,%edx
    13f1:	64 8b 04 25 f8 ff ff 	mov    %fs:0xfffffffffffffff8,%eax
    13f8:	ff 
    13f9:	89 c6                	mov    %eax,%esi
    13fb:	48 8d 05 22 0c 00 00 	lea    0xc22(%rip),%rax        # 2024 <_IO_stdin_used+0x24>
    1402:	48 89 c7             	mov    %rax,%rdi
    1405:	b8 00 00 00 00       	mov    $0x0,%eax
    140a:	e8 91 fc ff ff       	call   10a0 <printf@plt>
  }
  stackoverflow(n + 1);
    140f:	8b 45 dc             	mov    -0x24(%rbp),%eax
    1412:	83 c0 01             	add    $0x1,%eax
    1415:	89 c7                	mov    %eax,%edi
    1417:	e8 7b ff ff ff       	call   1397 <stackoverflow>
}
    141c:	90                   	nop
    141d:	48 8b 5d f8          	mov    -0x8(%rbp),%rbx
    1421:	c9                   	leave  
    1422:	c3                   	ret    

0000000000001423 <Tprobe>:

void Tprobe(int tid) {
    1423:	f3 0f 1e fa          	endbr64 
    1427:	55                   	push   %rbp
    1428:	48 89 e5             	mov    %rsp,%rbp
    142b:	48 83 ec 10          	sub    $0x10,%rsp
    142f:	89 7d fc             	mov    %edi,-0x4(%rbp)
  id = tid;
    1432:	8b 45 fc             	mov    -0x4(%rbp),%eax
    1435:	64 89 04 25 f8 ff ff 	mov    %eax,%fs:0xfffffffffffffff8
    143c:	ff 
  base = (void *)&tid;
    143d:	48 8d 45 fc          	lea    -0x4(%rbp),%rax
    1441:	64 48 89 04 25 e8 ff 	mov    %rax,%fs:0xffffffffffffffe8
    1448:	ff ff 
  stackoverflow(0);
    144a:	bf 00 00 00 00       	mov    $0x0,%edi
    144f:	e8 43 ff ff ff       	call   1397 <stackoverflow>
}
    1454:	90                   	nop
    1455:	c9                   	leave  
    1456:	c3                   	ret    

0000000000001457 <main>:

int main() {
    1457:	f3 0f 1e fa          	endbr64 
    145b:	55                   	push   %rbp
    145c:	48 89 e5             	mov    %rsp,%rbp
    145f:	48 83 ec 10          	sub    $0x10,%rsp
  setbuf(stdout, NULL);
    1463:	48 8b 05 b6 2b 00 00 	mov    0x2bb6(%rip),%rax        # 4020 <stdout@GLIBC_2.2.5>
    146a:	be 00 00 00 00       	mov    $0x0,%esi
    146f:	48 89 c7             	mov    %rax,%rdi
    1472:	e8 19 fc ff ff       	call   1090 <setbuf@plt>
  for (int i = 0; i < 4; i++) {
    1477:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
    147e:	eb 13                	jmp    1493 <main+0x3c>
    create(Tprobe);
    1480:	48 8d 05 9c ff ff ff 	lea    -0x64(%rip),%rax        # 1423 <Tprobe>
    1487:	48 89 c7             	mov    %rax,%rdi
    148a:	e8 6b fd ff ff       	call   11fa <create>
  for (int i = 0; i < 4; i++) {
    148f:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
    1493:	83 7d fc 03          	cmpl   $0x3,-0x4(%rbp)
    1497:	7e e7                	jle    1480 <main+0x29>
    1499:	b8 00 00 00 00       	mov    $0x0,%eax
  }
}
    149e:	c9                   	leave  
    149f:	c3                   	ret    

Disassembly of section .fini:

00000000000014a0 <_fini>:
    14a0:	f3 0f 1e fa          	endbr64 
    14a4:	48 83 ec 08          	sub    $0x8,%rsp
    14a8:	48 83 c4 08          	add    $0x8,%rsp
    14ac:	c3                   	ret    
