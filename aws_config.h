const char* ssid = "Cyrus";
const char* password = "TlimsTsnwP23a!?J316!";

const char JSONPAYLOADhum[] = "{ \"SystemID\" : \"%s\", \"timestamp\": %lu, \"readingType\" : \"%s\",  \"ReadingValue\": %2f}";
const char JSONPAYLOADtemp[] = "{ \"SystemID\" : \"%s\", \"timestamp\": %lu, \"readingType\" : \"%s\",  \"ReadingValue\": %2f}";
const char JSONPAYLOADCO2[] = "{ \"SystemID\" : \"%s\", \"timestamp\": %lu, \"readingType\" : \"%s\",  \"ReadingValue\": %2f}";
const char JSONPAYLOADliq[] = "{ \"SystemID\" : \"%s\", \"timestamp\": %lu, \"readingType\" : \"%s\",  \"ReadingValue\": %2f}";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint 
const char* awsEndpoint = "a1zd057ad6b8i1-ats.iot.us-east-1.amazonaws.com";

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
"MIIDWTCCAkGgAwIBAgIUO/a2obUI0FOG8HHcXvEP2NOZCLMwDQYJKoZIhvcNAQEL" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDUwNTAyNDQx" \
"MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMOfZk95dD/3GL0uXOXF" \
"emzuE2K5OGmsxkW0+vnJSsr4SsK5liIlX0oJFlrkMUcP8qhIb+H4a/TKQgEF+ZTh" \
"viqjJRIV8CiPkdt7WAgv+gyRBzUhlqEiFgIURv52C50F5hRfXmqAapvSwITzegZF" \
"zcUAhttlZb9Yrv1mb4oMlZcYNLAs2DNmzzwxFxh25m+IKODC5pKVorOxbtaD7SsI" \
"NEbwKe0wF/Rvp4BwQjQKYh90pN8ZyorP/XIvcGeQdGEPksLi412GEy6zhtZHZLxW" \
"KisIkcU6V1Ub8siDBgdG52PpZJFMRTfy0pgc0vL79QrMTs6UHgGUWXG8YIoLts3X" \
"+yECAwEAAaNgMF4wHwYDVR0jBBgwFoAUkj+I94YcQqqnbcGjKc6Cab7JOrAwHQYD" \
"VR0OBBYEFNX6jlxZRzy9zv84qFmqnCCwjGtiMAwGA1UdEwEB/wQCMAAwDgYDVR0P" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBpLda2kTOV7triPs3ZQoo81AXW" \
"4iozrj2SyppLorVJZJNcTX8kM9kaIfwtq8RuJGzGctBDKLvAlwGR32uKpmHI6VH9" \
"2LBQiiV+J5P+3AuAENx3N9t+oHP7dC0d/57plh44lwKZ5iCw+7BEP+1qRVwp4vSA" \
"rCC9Xv2hYfswBfNqswBU1Non5tTknvHXj9oDTF8MAUhtMif30lwI6kO996c8lqWR" \
"GxNJ5e9nETFAGGPKFLfu/PKpJw3nnCQIAWNX2rJYuguSsiaIEYL07aVen2BGx3ys" \
"tRcuWb5aGpzAaf4Ali3gdOTjEfIIpjXXMl39aNCiupkknV43IOiolqM6u9Ap";
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
"MIIEpAIBAAKCAQEAw59mT3l0P/cYvS5c5cV6bO4TYrk4aazGRbT6+clKyvhKwrmW" \
"IiVfSgkWWuQxRw/yqEhv4fhr9MpCAQX5lOG+KqMlEhXwKI+R23tYCC/6DJEHNSGW" \
"oSIWAhRG/nYLnQXmFF9eaoBqm9LAhPN6BkXNxQCG22Vlv1iu/WZvigyVlxg0sCzY" \
"M2bPPDEXGHbmb4go4MLmkpWis7Fu1oPtKwg0RvAp7TAX9G+ngHBCNApiH3Sk3xnK" \
"is/9ci9wZ5B0YQ+SwuLjXYYTLrOG1kdkvFYqKwiRxTpXVRvyyIMGB0bnY+lkkUxF" \
"N/LSmBzS8vv1CsxOzpQeAZRZcbxgigu2zdf7IQIDAQABAoIBAQCneM0z+LxnDnml" \
"bA2nv/phSyhZekwrW/iaMKg0M/nN7ebC8fLv5KLBVSpIQddWCPcqzmjKQczaUlVT" \
"XvQeW2xcQDM6WoGxl8e86MmiqMAfBgHvoIO+CtpgiU+GBTVIBobEpijEcI/UOwcK" \
"Sw8KU7QnlZ3eMd57g8a7tXiD7OYb5OPglqHBXgCDWCU8K9Ms/bwxiKxcHL6P48Ud" \
"XmpCsz2gvXtT7rYoF9PctL9c7AkTFNIFd52HQ9n7eFYkWk333A/6c+IuXlosgQNg" \
"SDWw/ZqVHvkyflQ5i3AK+VTNNyaS/GtaCRYNgBLZTgPWwe+BcvIP2NB9G6Y7nuA4" \
"MpxfS3XhAoGBAOUXOcZ7jSfhKx5JKuAu/+4vfgoThHYk/WycB9azVJpSGJ8wISlD" \
"JSoLc9xKAXJJlYNsQYvtAhuNogJFubC3GcOlDAcbPb0gOVndCwzkOhLIhplV0ldq" \
"IKqKmaPgd/u/Ls8I+6iP4YlK8uiCwAckSfvZYtZxZ65vUmjGo2KdJA0dAoGBANqZ" \
"yWZ1Vb50lMHc+KygQV9xy+334CqltQ22Xd2S1+RwhBQ8gYygFt3pdkweIGFoSG9T" \
"NT0m/fDvUbbSzNo7EOKpRQyF4eh7VLc4Yvshw/oGRLDR7Z9QpregeIdA9AtYWjBk" \
"PU1C4enxINb4UzBwF+wG1l5zU/A69cpuf9Mb+rrVAoGAVS8is5Qjc629a8CHKd8w" \
"GTAdrL9/JLMybJ1EKOh7z/ow35h07DPkcNLbXRFDomyMJNcvNEmH3Bj5IeEtX/Fl" \
"qiMH6uiNN3LxAGI6Bqwij/NyYTfRQ+HZjbCF2rLPypNXY3KSqSX/Fh8BXcWog79R" \
"p004/EYk6G7KdN1PyY5m5l0CgYAP7H/1TLrQQuHLNXOP76STdVCrpxDXSnbj5tLz" \
"nwzHAUM9ziKJweeohqctaExnjBcqtN7982cwM965+KUfiIn0WOxEu0P3kXRcXGkp" \
"bg4+mlJKpyAuKMvcOH5PK/YbFXaMa+DjTenzBrOjtTcqIiA7+0uGWdRVfZYGBwKc" \
"uD8NeQKBgQDL9tJg6MkEtiqxc/Yy/dOgUNtaP2JpfChNoGaf8DgsCf+JdQqHIzvw" \
"wJrnGGDhJ/G8aWtk7xx5nND1y0/gh5TpqR1wOdjSGQLy96uyY9oPfA4gUfNp3PSr" \
"8fZTlsObUpk5G+DR6YVJHEAUHljT45QnMD8jW1naXHVBZCGfTYO7Ww==";

//-----END RSA PRIVATE KEY-----

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy" \
"rqXRfboQnoZsG4q5WTP468SQvvG5";
//-----END CERTIFICATE-----
