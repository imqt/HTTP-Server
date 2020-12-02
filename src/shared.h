#ifndef SHARED_H
#define SHARED_H

// shared between server and client

#define BUF_SIZE 4096

// https://www.sciencedirect.com/topics/computer-science/registered-port#:~:text=Ports%200%20through%201023%20are,be%20used%20dynamically%20by%20applications.
// /etc/services
#define PORT 49517

// Audio 	audio/mpeg
//          audio/mpeg3
// 			audio/x-ms-wma
// 			audio/vnd.rn-realaudio
// 			audio/x-wav

// Image 	image/gif
// 			image/jpeg
// 			image/png
// 			image/tiff
// 			image/vnd.microsoft.icon
// 			image/x-icon
// 			image/vnd.djvu
// 			image/svg+xml

// Multipart multipart/mixed
// 			multipart/alternative
// 			multipart/related (using by MHTML (HTML mail).)
// 			multipart/form-data

// Text 	text/css  so apparently most text stuffs can just be sent as text/html
// 			text/csv
// 			text/html
// 			text/javascript (obsolete)
// 			text/plain
// 			text/xml

// Video 	video/mpeg
// 			video/mp4
// 			video/quicktime
// 			video/x-ms-wmv
// 			video/x-msvideo
// 			video/x-flv
// 			video/webm 

#endif
