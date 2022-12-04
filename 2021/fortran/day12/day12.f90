MODULE Utils
  IMPLICIT NONE
  INTEGER,PARAMETER :: START=0,SMALL=1,LARGE=2,ENDER=3
  CONTAINS
    SUBROUTINE addNode(n,nNames,nodeType,nodeCtr,nodeIdx)
      CHARACTER(LEN=5),INTENT(IN) :: n
      CHARACTER(LEN=5),INTENT(INOUT) :: nNames(:)
      INTEGER(4),INTENT(INOUT) :: nodeType(:),nodeCtr
      INTEGER(4),INTENT(OUT) :: nodeIdx

      nodeIdx=FINDLOC(nNames,n,DIM=1)
      IF (nodeIdx/=0) RETURN
      nodeCtr = nodeCtr+1
      nodeIdx = nodeCtr
      SELECT CASE(n)
      CASE('start')
        nodeType(nodeIdx)=START
      CASE('end')
        nodeType(nodeIdx)=ENDER
      CASE DEFAULT
        IF (ICHAR(n(1:1))<91) THEN
          nodeType(nodeIdx) = LARGE
        ELSE
          nodeType(nodeIdx) = SMALL
        ENDIF
      ENDSELECT
      nNames(nodeIdx)=n
    ENDSUBROUTINE addNode

    RECURSIVE SUBROUTINE findRoute(routeStack,rPtr,adjMat,nodeType,dback,ret)
      INTEGER(4),INTENT(INOUT) :: routeStack(:),rPtr,ret
      LOGICAL,INTENT(IN) :: adjMat(:,:)
      INTEGER(4),INTENT(IN) :: nodeType(:)
      LOGICAL,INTENT(IN) :: dback
      INTEGER(4) :: dest
      LOGICAL :: dbackChild
      ! Base case: if top of stack is end, increment and return
      IF (nodeType(routeStack(rPtr))==ENDER) THEN
        ret = ret+1
        RETURN
      ENDIF
      ! Look for routes away from current node
      DO dest=1,SIZE(nodeType)
        IF (.NOT. adjMat(routeStack(rPtr),dest)) CYCLE
        ! Validate destination
        dbackChild=dback
        IF (nodeType(dest)==SMALL) THEN
          IF (FINDLOC(routeStack(1:rPtr),dest,DIM=1)/=0) THEN
            IF (dback) CYCLE
            dbackChild=.TRUE.
          ENDIF
        ENDIF
        ! Push onto stack
        rPtr=rPtr+1
        IF (rPtr>SIZE(routeStack)) STOP 123
        routeStack(rPtr)=dest
        ! Recursive call to traverse this route
        CALL findRoute(routeStack,rPtr,adjmat,nodeType,dbackChild,ret)
        ! Pop and move on
        rPtr=rPtr-1
      ENDDO
    ENDSUBROUTINE findRoute
ENDMODULE

PROGRAM Day12
  USE Utils
  IMPLICIT NONE
  INTEGER(4) :: nNodes,rPtr,ret,iNode,jNode,ierr
  INTEGER(4),ALLOCATABLE :: nodeType(:),routeStack(:)
  LOGICAL,ALLOCATABLE :: adjMat(:,:)
  CHARACTER(LEN=5) :: n1,n2
  CHARACTER(LEN=5),ALLOCATABLE :: nodeName(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nNodes
  ALLOCATE(nodeType(nNodes),adjMat(nNodes,nNodes))
  ALLOCATE(nodeName(nNodes))
  ALLOCATE(routeStack(2*nNodes))
  ! Build adjacency matrix. Must ensure "End" is last
  nNodes=0
  adjMat=.FALSE.
  DO WHILE(.TRUE.)
    READ(98,*,IOSTAT=ierr) n1,n2
    IF (ierr/=0) EXIT
    CALL addNode(n1,nodeName,nodeType,nNodes,iNode)
    CALL addNode(n2,nodeName,nodeType,nNodes,jNode)
    adjMat(iNode,jNode)=.TRUE.
    adjMat(jNode,iNode)=.TRUE.
  ENDDO
  !nNodes=nNodes+1 ! Account for end node being last
  ! Snip outgoing from END, incoming to START
  iNode = FINDLOC(nodeType,START,DIM=1)
  adjMat(:,iNode)=.FALSE.
  iNode = FINDLOC(nodeType,ENDER,DIM=1)
  adjMat(iNode,:)=.FALSE.

  DO iNode=1,nNodes
    WRITE(*,*) nodeName(iNode),"(",nodeType(iNode),"):",adjMat(iNode,:)
  ENDDO

  ! Push starting nodes onto stack
  ret = 0 
  rPtr = 1
  routeStack(1) = FINDLOC(nodeType,START,DIM=1)
  CALL findRoute(routeStack,rPtr,adjMat,nodetype,.TRUE.,ret)
  WRITE(*,*) "Found",ret,"paths (visit small caves once)"
  ret = 0 
  rPtr = 1
  routeStack(1) = FINDLOC(nodeType,START,DIM=1)
  CALL findRoute(routeStack,rPtr,adjMat,nodetype,.FALSE.,ret)
  WRITE(*,*) "Found",ret,"paths (visit one small cave twice)"
ENDPROGRAM Day12
