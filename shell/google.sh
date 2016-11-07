#!/bin/sh

user_agent="Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36"
sorry_url="https://ipv4.google.com/sorry/index?continue=https://www.google.com/search%3Fq%3Dlove%2Bsite:youku.com%26start%3D1%26num%3D20&q=CGMSBDTF9NcY4MiSwAUiGQDxp4NLX8tO-c1ggiShWl6H_2E6VupFPFI"

curl -L "$sorry_url" -H "x-client-data:CIy2yQEIorbJAQ==" -H "upgrade-insecure-requests:1" -H "user-agent: $user_agent" -o 1.html 
img_x=$(grep 'img src' 1.html | sed -n '/src[[:space:]]*=/{s/.*src[[:space:]]*=\"\(.*\?\)\"\sborder.*/\1/;p;q}')
echo $img_x
img_y=$(echo $img_x | sed -e 's/amp;//g')
echo $img_y
captcha_url="https://ipv4.google.com/$img_y&hl=zh-CN"
echo $captcha_url

curl -L "$captcha_url" -H "referer: $sorry_url" -H "x-client-data:CIy2yQEIorbJAQ==" -H "upgrade-insecure-requests:1" -H "user-agent: $user_agent" -o 2.jpeg 

read captcha
echo $captcha

url="https://ipv4.google.com/sorry/index?captcha=${captcha}&continue=https%3A%2F%2Fwww.google.com%2Fsearch%3Fq%3Dlove%2Bsite%3Ayouku.com%26start%3D1%26num%3D20&submit=Submit"
echo $url
curl -L "$url" -H "referer: $sorry_url" -H "x-client-data:CIy2yQEIorbJAQ==" -H "upgrade-insecure-requests:1" -H "user-agent: $user_agent" -o 3.html
