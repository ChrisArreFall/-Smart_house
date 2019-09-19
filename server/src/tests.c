#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[] )
{ FILE *fp;
	char path[1035];
	
	/* Open the command for reading. */
	fp = popen("ls images/", "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}

	// If there are no images well shit
	if (fgets(path, sizeof(path)-1, fp) == NULL){
		
	}
	else{

		// Numerate imgs
		int i = 1;
		char is[5];

		// Build starter part
		system("echo '<!doctype html><htmllang=\"en\"><head><style>.row>.column{padding:08px;}.row:after{content:\"\";display:table;clear:both;}/*Createfourequalcolumnsthatfloatsnexttoeachother*/.column{float:left;width:25%;}/*TheModal(background)*/.modal{display:none;position:fixed;z-index:1;padding-top:100px;left:0;top:0;width:100%;height:100%;overflow:auto;background-color:black;}/*ModalContent*/.modal-content{position:relative;background-color:#fefefe;margin:auto;padding:0;width:90%;max-width:1200px;}/*TheCloseButton*/.close{color:white;position:absolute;top:10px;right:25px;font-size:35px;font-weight:bold;}.close:hover,.close:focus{color:#999;text-decoration:none;cursor:pointer;}/*Hidetheslidesbydefault*/.mySlides{display:none;}/*Next&previousbuttons*/.prev,.next{cursor:pointer;position:absolute;top:50%;width:auto;padding:16px;margin-top:-50px;color:white;font-weight:bold;font-size:20px;transition:0.6sease;border-radius:03px3px0;user-select:none;-webkit-user-select:none;}/*Positionthe\"nextbutton\"totheright*/.next{right:0;border-radius:3px003px;}/*Onhover,addablackbackgroundcolorwithalittlebitsee-through*/.prev:hover,.next:hover{background-color:rgba(0,0,0,0.8);}/*Numbertext(1/3etc)*/.numbertext{color:#f2f2f2;font-size:12px;padding:8px12px;position:absolute;top:0;}/*Captiontext*/.caption-container{text-align:center;background-color:black;padding:2px16px;color:white;}img.demo{opacity:0.6;}.active,.demo:hover{opacity:1;}img.hover-shadow{transition:0.3s;}.hover-shadow:hover{box-shadow:04px8px0rgba(0,0,0,0.2),06px20px0rgba(0,0,0,0.19);}</style><metacharset=\"utf-8\"><title>Backyardcamera</title><metaname=\"description\"content=\"Backyardcamera\"><metaname=\"author\"content=\"SitePoint\"><linkrel=\"stylesheet\"href=\"css/styles.css?v=1.0\"></head><body>' > images.html");

		// Start division
		system("echo '<div class=\"row\">' >> images.html");

		// Add images to the viewer
		while (fgets(path, sizeof(path)-1, fp) != NULL) {	

			// Fill the html
			char html[1000] = "echo '<div class=\"column\"><img src=\"images/";
			// Convert int to char
			snprintf(is, sizeof(is), "%d", i);
			strcat(html, path);
			strcat(html, "\" onclick=\"openModal();currentSlide(");
			strcat(html, is);
			strcat(html , ")\" class=\"hover-shadow\"></div>' >> images.html");
			system(html);

			++i;

		}

		// Close the div
		system("echo '</div>' >> images.html");

		// Start the div
		system("echo '<div id=\"myModal\" class=\"modal\"><span class=\"close cursor\" onclick=\"closeModal()\">&times;</span><div class=\"modal-content\">' >> images.html");

		i = 1;
		// Re read all
		fp = popen("ls images/", "r");
		int total;
  		FILE *ptotal = popen("ls images/ | wc -l", "r");
		fscanf(ptotal, "%d", &total);
		// Add images to the viewer
		while (fgets(path, sizeof(path)-1, fp) != NULL) {

			// Fill the html
			char html[1000] = "echo '<div class=\"mySlides\"><div class=\"numbertext\">";
			// Convert int to char
			snprintf(is, sizeof(is), "%d", i);
			strcat(html, is);
			strcat(html, "/ ");
			// Convert int to char
			snprintf(is, sizeof(is), "%d", total);
			strcat(html, is);
			strcat(html, "</div><img src=\"images/");
			strcat(html, path);
			strcat(html , "\" style=\"width:100%\"></div>' >> images.html");

			// Call the command
			system(html);

			++i;
		}

		// Sides
		system("echo '<a class=\"prev\" onclick=\"plusSlides(-1)\">&#10094;</a><a class=\"next\" onclick=\"plusSlides(1)\">&#10095;</a>' >> images.html");
		// Caption
		system("echo '<div class=\"caption-container\"><p id=\"caption\"></p></div>' >> images.html");

		i = 1;
		// Re read all
		fp = popen("ls images/", "r");
		// Add images to the viewer
		while (fgets(path, sizeof(path)-1, fp) != NULL) {

			// Fill the html
			char html[1000] = "echo '<div class=\"column\"><img class=\"demo\" src=\"images/";
			strcat(html, path);
			strcat(html , "\" onclick=\"currentSlide(");
			// Convert int to char
			snprintf(is, sizeof(is), "%d", i);
			strcat(html, is);
			strcat(html, ")\" alt=\"");
			strcat(html, path);
			strcat(html, "\"></div>' >> images.html");

			// Call the command
			system(html);

			++i;
		}
		// Send 9 to go back
		system("echo '</div></div><button type=\"button\" class=\"btnBack\" onclick=httpPOST(9)>Back to main menu</button> </body></html>' >> images.html");
		// OCUPA EL IP AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
		system("echo '<script> function openModal() {document.getElementById(\"myModal\").style.display = \"block\";}function closeModal() {document.getElementById(\"myModal\").style.display = \"none\";}var slideIndex = 1;showSlides(slideIndex);function plusSlides(n) {showSlides(slideIndex += n);}function currentSlide(n) {showSlides(slideIndex = n);}function showSlides(n) {var i;var slides = document.getElementsByClassName(\"mySlides\");var dots = document.getElementsByClassName(\"demo\");var captionText = document.getElementById(\"caption\");if (n > slides.length) {slideIndex = 1}if (n < 1) {slideIndex = slides.length}for (i = 0; i < slides.length; i++) {slides[i].style.display = \"none\";}for (i = 0; i < dots.length; i++) {dots[i].className = dots[i].className.replace(\" active\", "");}slides[slideIndex-1].style.display = \"block\";dots[slideIndex-1].className += \" active\";captionText.innerHTML = dots[slideIndex-1].alt;}function httpPOST(data){var xhr = new XMLHttpRequest();xhr.withCredentials = true;xhr.addEventListener(\"readystatechange\", function () {if (this.readyState === 4) {console.log(this.responseText);}});xhr.open(\"POST\", \"http://192.168.100.92:3000\");xhr.setRequestHeader(\"cache-control\", \"no-cache\");xhr.setRequestHeader(\"Postman-Token\", \"bb2d356c-4f5a-4baa-91eb-a6a9bf0a238d\"); xhr.send(data);console.log(\"Printed!!!\")}</script>' >> images.html");

	}

	/* close */
	pclose(fp);
	
}
	