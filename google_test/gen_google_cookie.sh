#!/bin/sh

username=${1:-alanshallowayjames@gmail.com}
password=${2:-xmf@he1krg}
cookie=${3:-./google_cookie}

username=${username//@/%40}
password=${password//@/%40}

user_agent="Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:44.0) Gecko/20100101 Firefox/44.0"
content_type_header="Content-Type: application/x-www-form-urlencoded"

login_url="https://accounts.google.com/ServiceLogin?hl=en"
curl $login_url -H "User-Agent: ${user_agent}" -k -c ${cookie} -o 1.html

GALX=`grep 'GALX' 1.html | sed -n '/value=/{s/.*value=\"\([[:alnum:]]*\)\">/\1/;p;q}'`
gxf=`grep 'gxf' 1.html | sed -n '/value=/{s/.*value=\"\(.*\?\)\">/\1/;p;q}'`

# url encode
requestlocation=${login_url//:/%3A}
requestlocation=${requestlocation//\//%2F}
requestlocation=${requestlocation//=/%3D}
requestlocation=${requestlocation//\&/%26}
requestlocation=${requestlocation//\?/%3F}
requestlocation=${requestlocation}%23identifier

curl "https://accounts.google.com/accountLoginInfoXhr" \
    -H "${content_type_header}" \
    -H "Referer: ${login_url}" \
    -H "User-Agent: ${user_agent}" \
    --data "Email=${username}&requestlocation=${requestlocation}&bgresponse=js_disabled&Page=RememberedSignIn&GALX=${GALX}&gxf=${gxf}&hl=en&_utf8=%E2%98%83&pstMsg=1&checkConnection=youtube%3A1127%3A1&checkedDomains=youtube&rmShown=1" \
    -k -c ${cookie} -b ${cookie} -o 2.html
ProfileInformation=`cat 2.html |sed -n '/encoded_profile_information/{s/.*encoded_profile_information\":\"\(.*\?\)\",.*/\1/;p;q}'`

# get cookie
curl "https://accounts.google.com/ServiceLoginAuth" \
    -H "Referer: ${login_url}" \
    -H "User-Agent: ${user_agent}" \
    -H "${content_type_header}" \
    --data "Page=RememberedSignIn&GALX=${GALX}&gxf=${gxf}&hl=en&_utf8=%E2%98%83&pstMsg=1&dnConn=&checkConnection=youtube%3A1127%3A1&checkedDomains=youtube&identifiertoken=&identifiertoken_audio=&identifier-captcha-input=&Email=${username}&Passwd=${password}&PersistentCookie=yes&ProfileInformation=${ProfileInformation}&rmShown=1&bgresponse=js_disabled" \
    -k -c ${cookie} -b ${cookie} -o /dev/null

rm -rf *.html
