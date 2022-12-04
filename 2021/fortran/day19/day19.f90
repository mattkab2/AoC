MODULE Utils
  IMPLICIT NONE
  INTEGER,PARAMETER :: xp = 1, xm = -1, yp = 2, ym = -2, zp = 3, zm = -3
  TYPE,PUBLIC :: Scanner
    INTEGER(4) :: tform(3)=(/xp,yp,zp/)
    INTEGER(8) :: nBeacons
    INTEGER(8),ALLOCATABLE :: scanDat(:,:),relDat(:,:,:)
    CONTAINS
      PROCEDURE,PASS :: init => s_init
      PROCEDURE,PASS :: rotate => s_rotate
      PROCEDURE,PASS :: translate => s_translate
      PROCEDURE,PASS :: merge => s_merge
  ENDTYPE
  CONTAINS
    SUBROUTINE s_init(this,dat)
      CLASS(Scanner),INTENT(INOUT) :: this
      INTEGER(8),INTENT(IN) :: dat(:,:)
      INTEGER(8) :: i,j

      this%scanDat = dat
      this%nBeacons = SIZE(dat,DIM=1)
      ALLOCATE(this%relDat(SIZE(dat,DIM=1),SIZE(dat,DIM=1),3))
      this%relDat=HUGE(this%relDat(1,1,1))
      DO i=1,SIZE(dat,DIM=1)
        DO j=1,i-1
          this%relDat(i,j,:) = dat(i,:)-dat(j,:)
        ENDDO
      ENDDO
    ENDSUBROUTINE s_init
    SUBROUTINE s_rotate(this,fDir,uDir,lRel,lScan)
      CLASS(Scanner),INTENT(INOUT) :: this
      INTEGER(4),INTENT(IN) :: fDir,uDir
      LOGICAL,INTENT(IN) :: lRel,lScan
      INTEGER(4) :: shift,nDir

      SELECT CASE(ABS(fDir*uDir))
      CASE( 2); nDir= SIGN(3,fDir*uDir)
      CASE( 3); nDir= SIGN(2,fDir*uDir)
      CASE DEFAULT; nDir= SIGN(1,fDir*uDir)
      ENDSELECT
      IF ((.NOT. ABS(fDir)<ABS(nDir) .OR. (ABS(fDir)==3 .AND. ABS(uDir)==1))) nDir= -nDir
      IF (lRel) THEN
        ! Shift forward coord to correct position
        IF (ABS(this%tform(1))/=ABS(fDir)) THEN
          shift = ABS(ABS(fDir) - ABS(this%tform(1)))
          this%relDat = CSHIFT(this%relDat,shift,DIM=3)
          this%tform = CSHIFT(this%tform,shift)
        ENDIF
        ! Check if we need to swap normal directions
        IF (ABS(this%tform(3)) /= ABS(uDir)) THEN
          this%relDat(:,:,2:3) = CSHIFT(this%relDat(:,:,2:3),1,DIM=3)
          this%tform(2:3) = CSHIFT(this%tform(2:3),1)
        ENDIF
        ! Check signs
        IF (fDir/=this%tform(1)) THEN
          this%tform(1) = -this%tform(1)
          this%relDat(:,:,1) = -this%relDat(:,:,1)
        ENDIF
        IF (nDir/=this%tform(2)) THEN
          this%tform(2) = -this%tform(2)
          this%relDat(:,:,2) = -this%relDat(:,:,2)
        ENDIF
        IF (uDir/=this%tform(3)) THEN
          this%tform(3) = -this%tform(3)
          this%relDat(:,:,3) = -this%relDat(:,:,3)
        ENDIF
      ENDIF
    ENDSUBROUTINE s_rotate
    SUBROUTINE s_translate(this,dx,dy,dz)
      CLASS(Scanner),INTENT(INOUT) :: this
      INTEGER(8),INTENT(IN) :: dx,dy,dz

      this%scanDat(:,1) = this%scanDat(:,1) + dx
      this%scanDat(:,2) = this%scanDat(:,1) + dy
      this%scanDat(:,3) = this%scanDat(:,1) + dz
      
      this%relDat(:,:,1) = this%relDat(:,:,1) + dx
      this%relDat(:,:,2) = this%relDat(:,:,2) + dy
      this%relDat(:,:,3) = this%relDat(:,:,3) + dz
    ENDSUBROUTINE s_translate
    SUBROUTINE s_merge(this,s,dup)
      CLASS(Scanner),INTENT(INOUT) :: this
      TYPE(Scanner),INTENT(INOUT) :: s
      LOGICAL,INTENT(IN) :: dup(:)
      INTEGER(8) :: i,j,idx,jdx
      INTEGER(8),ALLOCATABLE :: tmp1(:,:),tmp2(:,:,:)

      IF (s%tform(1)/=this%tform(1) .OR. s%tform(2)/=this%tform(2) .OR. s%tform(3)/=this%tform(3)) STOP 123
      ALLOCATE(tmp1(this%nBeacons+COUNT(.NOT. dup),3))
      tmp1(:this%nBeacons,:) = this%scanDat(:,:)
      idx=this%nBeacons+1
      DO i=1,s%nBeacons
        IF (dup(i)) CYCLE
        tmp1(idx,:) = s%scanDat(i,:)
        idx = idx+1
      ENDDO
      DEALLOCATE(this%scanDat)
      this%scanDat=tmp1
      DEALLOCATE(tmp1)
      
      ALLOCATE(tmp2(this%nBeacons+COUNT(.NOT. dup),this%nBeacons+COUNT(.NOT. dup),3))
      tmp2 = HUGE(tmp2(1,1,1))
      tmp2(:this%nBeacons,:this%nBeacons,:) = this%relDat(:,:,:)
      idx=this%nBeacons+1
      DO i=1,s%nBeacons
        IF (dup(i)) CYCLE
        jdx=this%nBeacons+1
        DO j=1,i-1
          IF (dup(j)) CYCLE
          tmp2(idx,jdx,:) = this%relDat(idx,jdx,:)
          jdx=jdx+1
        ENDDO
        idx=idx+1
      ENDDO

      ! RelDat Fixup
      DO idx=this%nBeacons+1,SIZE(tmp2,DIM=1)
        DO jdx=1,this%nBeacons
          tmp2(idx,jdx,:) = this%scanDat(idx,:) - this%scanDat(jdx,:)
        ENDDO
      ENDDO
      DEALLOCATE(this%relDat)
      this%relDat=tmp2
      DEALLOCATE(tmp2)

      this%nBeacons = SIZE(this%scanDat,DIM=1)
      DEALLOCATE(s%scanDat)
      DEALLOCATE(s%relDat)
      s%nBeacons = 0
    ENDSUBROUTINE s_merge
    SUBROUTINE findMatches(s1,s2,dup)
      TYPE(Scanner),INTENT(INOUT) :: s1,s2
      LOGICAL,ALLOCATABLE,INTENT(OUT) :: dup(:)
      LOGICAL,ALLOCATABLE :: m1(:,:),m2(:,:)
      INTEGER(8) :: nMatches(24)=0,tmp(3),i,j
      INTEGER(8),ALLOCATABLE :: idxStor(:,:)
      INTEGER(4) :: fDir,uDir,tIdx,cnt,nDup
      REAL(4) :: nDupR

      tIdx = 1
      nMatches=0
      ALLOCATE(dup(s2%nBeacons))
      dup = .FALSE.
      outer: DO fDir=zm,zp
        DO uDir=zm,zp
          IF (fDir==0 .OR. uDir==0) CYCLE
          IF (ABS(uDir)==ABS(fDir)) CYCLE
          CALL s2%rotate(fDir,uDir,.TRUE.,.FALSE.)
          WRITE(*,*) "Under ",fDir,uDir
          WRITE(*,*) "  ",s2%relDat(2,1,:)
          DO i=1,s1%nBeacons
            DO j=1,i-1
              nMatches(tIdx) = nMatches(tIdx) + COUNT( &
                  s1%relDat(i,j,1)==s2%relDat(:,:,1) .AND. &
                  s1%relDat(i,j,2)==s2%relDat(:,:,2) .AND. &
                  s1%relDat(i,j,3)==s2%relDat(:,:,3))
              nMatches(tIdx) = nMatches(tIdx) + COUNT( &
                  s1%relDat(i,j,1)== -s2%relDat(:,:,1) .AND. &
                  s1%relDat(i,j,2)== -s2%relDat(:,:,2) .AND. &
                  s1%relDat(i,j,3)== -s2%relDat(:,:,3))
            ENDDO
          ENDDO
          IF (nMatches(tIdx) >= 66) EXIT outer
          tIdx = tIdx+1
        ENDDO
      ENDDO outer

      IF (ANY(nMatches >= 66)) THEN
        nDupR = 0.5*(SQRT(REAL(8*MAXVAL(nMatches)+1))-1)
        nDup = INT(nDupR)
        IF (nDup /= nDupR) THEN
          WRITE(*,*) MAXVAL(nMatches),nDupR,nDup
          STOP 234
        ENDIF
        DO i=s2%nBeacons,1,-1
          cnt = 0
          dup = .FALSE.
          DO j=1,i-1
            m1 =(s1%relDat(:,:,1)==s2%relDat(i,j,1) .AND. &
                 s1%relDat(:,:,2)==s2%relDat(i,j,2) .AND. &
                 s1%relDat(:,:,3)==s2%relDat(i,j,3))
            iDup = MAX(FINDLOC(m1,.TRUE.,DIM=1),iDup)
            jDup = MAX(FINDLOC(m1,.TRUE.,DIM=2),jDup)
            m2 =(s1%relDat(:,:,1)== -s2%relDat(i,j,1) .AND. &
                 s1%relDat(:,:,2)== -s2%relDat(i,j,2) .AND. &
                 s1%relDat(:,:,3)== -s2%relDat(i,j,3))
            cnt = cnt + COUNT(m1)+COUNT(m2)
            dup(j) = ANY(m1) .OR. ANY(m2)
          ENDDO
          IF (cnt == nDup) THEN
            dup(i) = .TRUE.
            ! Get translation
            dx = s1%scanDat(,1)-s2%scanDat(,1)
            dy = s1%scanDat(,2)-s2%scanDat(,2)
            dz = s1%scanDat(,3)-s2%scanDat(,3)
            !idx = FINDLOC(s1%relDat(:,:
            EXIT
          ENDIF
        ENDDO
        CALL s2%rotate(fDir,uDir,.FALSE.,.TRUE.)
        CALL s2%translate(
      ENDIF
    ENDSUBROUTINE findMatches
ENDMODULE
PROGRAM Day19
  USE Utils
  IMPLICIT NONE
  INTEGER(8) :: i,j,nScanner,bIdx,bJdx
  INTEGER(8),ALLOCATABLE :: beacons(:),beaconData(:,:)
  TYPE(Scanner),ALLOCATABLE :: scanners(:)
  LOGICAL,ALLOCATABLE :: nDup(:)

  OPEN(unit=98,file="input.dat")
  READ(98,*) nScanner
  ALLOCATE(scanners(nScanner))
  ALLOCATE(beacons(nScanner))
  READ(98,*) beacons

  DO i=1,nScanner
    ALLOCATE(beaconData(beacons(i),3))
    WRITE(*,*) "Scanner: ",i
    DO j=1,beacons(i)
      READ(98,*) beaconData(j,:)
      WRITE(*,*) beaconData(j,:)
    ENDDO
    CALL scanners(i)%init(beaconData)
    DEALLOCATE(beaconData)
  ENDDO

  outer: DO WHILE (.TRUE.)
    DO i=1,SIZE(scanners)
      DO j=1,i-1
        IF (scanners(i)%nBeacons==0 .OR. scanners(j)%nBeacons==0) CYCLE
        CALL findMatches(scanners(i),scanners(j),nDup)
        IF (ANY(nDup)) CALL scanners(i)%merge(scanners(j))
        EXIT outer
      ENDDO
    ENDDO
  ENDDO outer
  WRITE(*,*) "Success: ",nDup




ENDPROGRAM Day19
