#!/bin/sh

post_receive_tar_bz2() {
  if [ "$REQUEST_METHOD" = "POST" ]; then
    cat >$TMPOUT

    # Get the line count
    LINES=$(grep -c "" $TMPOUT)

    touch $TMPOUTbz2
    l=1
    LENSKIP=0

    # Process post data removing head and tail
    while true; do
      if [ $l -eq 1 ]; then
        ROW=$(cat $TMPOUT | awk "FNR == $l {print}")
        BOUNDARY=${#ROW}
        BOUNDARY=$((BOUNDARY + 1))
        LENSKIPSTART=$BOUNDARY
        LENSKIPEND=$BOUNDARY
      elif [ $l -le 4 ]; then
        ROW=$(cat $TMPOUT | awk "FNR == $l {print}")
        ROWLEN=${#ROW}
        LENSKIPSTART=$((LENSKIPSTART + ROWLEN + 1))
      elif [ \( $l -gt 4 \) -a \( $l -lt $LINES \) ]; then
        ROW=$(cat $TMPOUT | awk "FNR == $l {print}")
      else
        break
      fi
      l=$((l + 1))
    done
  fi

  # Extract tar.bz2 file
  LEN=$((CONTENT_LENGTH - LENSKIPSTART - LENSKIPEND + 2))
  dd if=$TMPOUT of=$TMPOUTbz2 bs=1 skip=$LENSKIPSTART count=$LEN >/dev/null 2>&1
  cd $TMPDIR
  tar jxvf $TMPOUTbz2 >/dev/null 2>&1
  RES=$?

  # Verify result of tar.bz2 command and copy files to destination
  if [ $RES -eq 0 ]; then
    if [ \( -f "system.conf" \) -a \( -f "camera.conf" \) ]; then
      mv -f *.conf /home/yi-hack/etc/
      chmod 0644 /home/yi-hack/etc/*.conf
      if [ -f hostname ]; then
        mv -f hostname /home/yi-hack/etc/
        chmod 0644 /home/yi-hack/etc/hostname
      fi
      RES=0
    else
      RES=1
    fi
  fi
}
