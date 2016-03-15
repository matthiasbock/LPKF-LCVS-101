/* -*- mode: C;-*--------------------------------------------------------------
 * (c)2009-2010 Copyright InfoStreet, Inc.
 * Use without the permission of InfoStreet Inc. is strictly prohibited.
 * Contact InfoStreet for more details (http://www.infostreet.com)
 *
 * Name:           $RCSfile: check_login.js,v $
 * Author:         Alex Soleymani <alex@infostreet.com>
 * Created On:     Dec 11, 2009
 * Last Modified:  $Date: 2010/01/27 01:22:39 $
 * Last Modifier:  $Author: alex $
 * Version:        $Revision: 1.2 $
 *
 *----------------------------------------------------------------------------*/

var ss_siteName = "";
var ss_loginDestination = "";
var ss_username = "";

function ss_initialize() {
  var thisHref = document.location.href;
  if(thisHref.substr(0, 6) == "https:") return;
  if (ss_siteName) {
    var s = document.createElement("script");
    s.type = "text/javascript";
    s.src = ss_siteName + "/login/check_login.pyt?getUsername=1";
    document.body.appendChild(s);
  } else {
    alert("ss_siteName is not defined");
  }
}

function ss_getUser(logedIn, username) {
  if (logedIn != "yes") {
    var ss_login_button = document.getElementById("SS_login_button");
    var registernowDiv = document.getElementById("RegisterNowDiv");
    if (ss_login_button) {
      ss_login_button.style.display = "block";
      registernowDiv.style.display = "block";
    } else {
      alert("Cannot find login button");
    }
  } else {
    ss_username = username;
    var ss_logout_button = document.getElementById("SS_logout_button");
	var usernameDiv = document.getElementById("UsernameDiv");
    var registernowDiv = document.getElementById("RegisterNowDiv");
    if (ss_logout_button) {
      ss_logout_button.style.display = "block";
	  usernameDiv.innerHTML = "Welcome, " + ss_username;
  	  usernameDiv.style.display = "block";
    } else {
      alert("Cannot find logout button");
    }
  }
}

function ss_login() {
  if ( ss_loginDestination == "self" ) {
    var loginUrl = ss_siteName + "/login/log_in.pyt?destination=" + document.location.href;
  } else if (ss_loginDestination) {
    if ( ss_loginDestination.substr(0, 4) != "http" ) {
      ss_loginDestination = "http://" + ss_loginDestination;
    }
    var loginUrl = ss_siteName + "/login/log_in.pyt?destination=" + ss_loginDestination;
  } else {
    var loginUrl = ss_siteName + "/login/";
  }
  document.location.href = loginUrl;
}

function ss_logout() {
  document.location.href = ss_siteName + "/login/logout.pyt";
}
