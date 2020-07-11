#!/bin/sh

YI_HACK_PREFIX="/home/yi-hack"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lib:/home/yi-hack/lib:/tmp/sd/yi-hack/lib
export PATH=$PATH:/home/base/tools:/home/yi-hack/bin:/home/yi-hack/sbin:/tmp/sd/yi-hack/bin:/tmp/sd/yi-hack/sbin

NAME="$(echo "$QUERY_STRING" | cut -d'=' -f1)"
VAL="$(echo "$QUERY_STRING" | cut -d'=' -f2)"

FW_VERSION=$(cat /home/yi-hack/version)
LATEST_FW=$(/home/yi-hack/usr/bin/wget -O - https://api.github.com/repos/roleoroleo/yi-hack-Allwinner/releases/latest 2>&1 | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')

TMPDIR=/tmp/sd/fw_upgrade
TMPOUT=/tmp/sd/uploaded_firmware.tar.bz2.dl
TMPOUTbz2=$TMPDIR/uploaded_firmware.tar.bz2

prepare_upgrade() {
  FREE_SD=$(df /tmp/sd/ | grep mmc | awk '{print $4}')
  if [ -z "$FREE_SD" ]; then
    printf "Content-type: text/html\r\n\r\n"
    printf "No SD detected."
    exit
  fi

  if [ "$FREE_SD" -lt 100000 ]; then
    printf "Content-type: text/html\r\n\r\n"
    printf "No space left on SD."
    exit
  fi

  # Clean old upgrades
  rm -rf /tmp/sd/fw_upgrade
  rm -rf /tmp/sd/Factory
  rm -rf /tmp/sd/newhome

  mkdir -p $TMPDIR
  cd $TMPDIR || exit
}

if [ "$REQUEST_METHOD" = "POST" ]; then

  # Cleanup
  rm -f $TMPOUT
  rm -f $TMPOUTbz2
  prepare_upgrade

  cat >"$TMPOUT"

  # Get the line count
  LINES=$(grep -c "" "$TMPOUT")

  touch "$TMPOUTbz2"
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

  # Extract tar.bz2 file
  LEN=$((CONTENT_LENGTH - LENSKIPSTART - LENSKIPEND + 2))
  dd if="$TMPOUT" of="$TMPOUTbz2" bs=1 skip=$LENSKIPSTART count=$LEN >/dev/null 2>&1

  VAL="upload"

elif [ "$NAME" != "get" ]; then
  exit
fi

perform_upgrade() {

  if [ ! -f "$1" ]; then
    printf "Content-type: text/html\r\n\r\n"
    printf "Unable to locate firmware file."
    exit
  fi

  tar zxvf "$1"
  rm "$1"
  cp -rf ./* ..
  rm -rf /tmp/sd/fw_upgrade/*
  cp -f $YI_HACK_PREFIX/etc/*.conf .
  if [ -f $YI_HACK_PREFIX/etc/hostname ]; then
    cp -f $YI_HACK_PREFIX/etc/hostname .
  fi

  # Report the status to the caller
  printf "Content-type: text/html\r\n\r\n"
  printf "Firmware file installed, rebooting and upgrading."

  sync
  sync
  sync
  sleep 1
  reboot
}

if [ "$VAL" == "info" ]; then
  printf "Content-type: application/json\r\n\r\n"

  printf "{\n"
  printf "\"%s\":\"%s\",\n" "fw_version" "$FW_VERSION"
  printf "\"%s\":\"%s\"\n" "latest_fw" "$LATEST_FW"
  printf "}"

elif [ "$VAL" == "upgrade" ]; then

  prepare_upgrade

  MODEL_SUFFIX=$(cat $YI_HACK_PREFIX/model_suffix)

  if [ "$FW_VERSION" == "$LATEST_FW" ]; then
    printf "Content-type: text/html\r\n\r\n"
    printf "No new firmware available."
    exit
  fi

  FW_FILENAME="${MODEL_SUFFIX}"_"${LATEST_FW}".tgz
  /home/yi-hack/usr/bin/wget https://github.com/roleoroleo/yi-hack-Allwinner/releases/download/"$LATEST_FW"/"$FW_FILENAME"
  if [ ! -f "$FW_FILENAME" ]; then
    printf "Content-type: text/html\r\n\r\n"
    printf "Unable to download firmware file."
    exit
  fi

  perform_upgrade "$FW_FILENAME"

elif [ "$VAL" == "upload" ]; then
  printf "Content-type: text/html\r\n\r\n"
  printf "Uploaded, processing..."

  perform_upgrade $TMPOUTbz2

fi
