//v1.7
// Flash Player Version Detection
// Detect Client Browser type
// Copyright 2005-2007 Adobe Systems Incorporated.  All rights reserved.
var isIE  = (navigator.appVersion.indexOf("MSIE") != -1) ? true : false;
var isWin = (navigator.appVersion.toLowerCase().indexOf("win") != -1) ? true : false;
var isOpera = (navigator.userAgent.indexOf("Opera") != -1) ? true : false;

function ControlVersion()
{
	var version;
	var axo;
	var e;

	// NOTE : new ActiveXObject(strFoo) throws an exception if strFoo isn't in the registry

	try {
		// version will be set for 7.X or greater players
		axo = new ActiveXObject("ShockwaveFlash.ShockwaveFlash.7");
		version = axo.GetVariable("$version");
	} catch (e) {
	}

	if (!version)
	{
		try {
			// version will be set for 6.X players only
			axo = new ActiveXObject("ShockwaveFlash.ShockwaveFlash.6");
			
			// installed player is some revision of 6.0
			// GetVariable("$version") crashes for versions 6.0.22 through 6.0.29,
			// so we have to be careful. 
			
			// default to the first public version
			version = "WIN 6,0,21,0";

			// throws if AllowScripAccess does not exist (introduced in 6.0r47)		
			axo.AllowScriptAccess = "always";

			// safe to call for 6.0r47 or greater
			version = axo.GetVariable("$version");

		} catch (e) {
		}
	}

	if (!version)
	{
		try {
			// version will be set for 4.X or 5.X player
			axo = new ActiveXObject("ShockwaveFlash.ShockwaveFlash.3");
			version = axo.GetVariable("$version");
		} catch (e) {
		}
	}

	if (!version)
	{
		try {
			// version will be set for 3.X player
			axo = new ActiveXObject("ShockwaveFlash.ShockwaveFlash.3");
			version = "WIN 3,0,18,0";
		} catch (e) {
		}
	}

	if (!version)
	{
		try {
			// version will be set for 2.X player
			axo = new ActiveXObject("ShockwaveFlash.ShockwaveFlash");
			version = "WIN 2,0,0,11";
		} catch (e) {
			version = -1;
		}
	}
	
	return version;
}

// JavaScript helper required to detect Flash Player PlugIn version information
function GetSwfVer(){
	// NS/Opera version >= 3 check for Flash plugin in plugin array
	var flashVer = -1;
	
	if (navigator.plugins != null && navigator.plugins.length > 0) {
		if (navigator.plugins["Shockwave Flash 2.0"] || navigator.plugins["Shockwave Flash"]) {
			var swVer2 = navigator.plugins["Shockwave Flash 2.0"] ? " 2.0" : "";
			var flashDescription = navigator.plugins["Shockwave Flash" + swVer2].description;
			var descArray = flashDescription.split(" ");
			var tempArrayMajor = descArray[2].split(".");			
			var versionMajor = tempArrayMajor[0];
			var versionMinor = tempArrayMajor[1];
			var versionRevision = descArray[3];
			if (versionRevision == "") {
				versionRevision = descArray[4];
			}
			if (versionRevision[0] == "d") {
				versionRevision = versionRevision.substring(1);
			} else if (versionRevision[0] == "r") {
				versionRevision = versionRevision.substring(1);
				if (versionRevision.indexOf("d") > 0) {
					versionRevision = versionRevision.substring(0, versionRevision.indexOf("d"));
				}
			}
			var flashVer = versionMajor + "." + versionMinor + "." + versionRevision;
		}
	}
	// MSN/WebTV 2.6 supports Flash 4
	else if (navigator.userAgent.toLowerCase().indexOf("webtv/2.6") != -1) flashVer = 4;
	// WebTV 2.5 supports Flash 3
	else if (navigator.userAgent.toLowerCase().indexOf("webtv/2.5") != -1) flashVer = 3;
	// older WebTV supports Flash 2
	else if (navigator.userAgent.toLowerCase().indexOf("webtv") != -1) flashVer = 2;
	else if ( isIE && isWin && !isOpera ) {
		flashVer = ControlVersion();
	}	
	return flashVer;
}

// When called with reqMajorVer, reqMinorVer, reqRevision returns true if that version or greater is available
function DetectFlashVer(reqMajorVer, reqMinorVer, reqRevision)
{
	versionStr = GetSwfVer();
	if (versionStr == -1 ) {
		return false;
	} else if (versionStr != 0) {
		if(isIE && isWin && !isOpera) {
			// Given "WIN 2,0,0,11"
			tempArray         = versionStr.split(" "); 	// ["WIN", "2,0,0,11"]
			tempString        = tempArray[1];			// "2,0,0,11"
			versionArray      = tempString.split(",");	// ['2', '0', '0', '11']
		} else {
			versionArray      = versionStr.split(".");
		}
		var versionMajor      = versionArray[0];
		var versionMinor      = versionArray[1];
		var versionRevision   = versionArray[2];

        	// is the major.revision >= requested major.revision AND the minor version >= requested minor
		if (versionMajor > parseFloat(reqMajorVer)) {
			return true;
		} else if (versionMajor == parseFloat(reqMajorVer)) {
			if (versionMinor > parseFloat(reqMinorVer))
				return true;
			else if (versionMinor == parseFloat(reqMinorVer)) {
				if (versionRevision >= parseFloat(reqRevision))
					return true;
			}
		}
		return false;
	}
}

function AC_AddExtension(src, ext)
{
  if (src.indexOf('?') != -1)
    return src.replace(/\?/, ext+'?'); 
  else
    return src + ext;
}

function AC_Generateobj(objAttrs, params, embedAttrs) 
{ 
  var str = '';
  if (isIE && isWin && !isOpera)
  {
    str += '<object ';
    for (var i in objAttrs)
    {
      str += i + '="' + objAttrs[i] + '" ';
    }
    str += '>';
    for (var i in params)
    {
      str += '<param name="' + i + '" value="' + params[i] + '" /> ';
    }
    str += '</object>';
  }
  else
  {
    str += '<embed ';
    for (var i in embedAttrs)
    {
      str += i + '="' + embedAttrs[i] + '" ';
    }
    str += '> </embed>';
  }

  document.write(str);
}

function AC_FL_RunContent(){
  var ret = 
    AC_GetArgs
    (  arguments, ".swf", "movie", "clsid:d27cdb6e-ae6d-11cf-96b8-444553540000"
     , "application/x-shockwave-flash"
    );
  AC_Generateobj(ret.objAttrs, ret.params, ret.embedAttrs);
}

function AC_SW_RunContent(){
  var ret = 
    AC_GetArgs
    (  arguments, ".dcr", "src", "clsid:166B1BCA-3F9C-11CF-8075-444553540000"
     , null
    );
  AC_Generateobj(ret.objAttrs, ret.params, ret.embedAttrs);
}

function AC_GetArgs(args, ext, srcParamName, classid, mimeType){
  var ret = new Object();
  ret.embedAttrs = new Object();
  ret.params = new Object();
  ret.objAttrs = new Object();
  for (var i=0; i < args.length; i=i+2){
    var currArg = args[i].toLowerCase();    

    switch (currArg){	
      case "classid":
        break;
      case "pluginspage":
        ret.embedAttrs[args[i]] = args[i+1];
        break;
      case "src":
      case "movie":	
        args[i+1] = AC_AddExtension(args[i+1], ext);
        ret.embedAttrs["src"] = args[i+1];
        ret.params[srcParamName] = args[i+1];
        break;
      case "onafterupdate":
      case "onbeforeupdate":
      case "onblur":
      case "oncellchange":
      case "onclick":
      case "ondblClick":
      case "ondrag":
      case "ondragend":
      case "ondragenter":
      case "ondragleave":
      case "ondragover":
      case "ondrop":
      case "onfinish":
      case "onfocus":
      case "onhelp":
      case "onmousedown":
      case "onmouseup":
      case "onmouseover":
      case "onmousemove":
      case "onmouseout":
      case "onkeypress":
      case "onkeydown":
      case "onkeyup":
      case "onload":
      case "onlosecapture":
      case "onpropertychange":
      case "onreadystatechange":
      case "onrowsdelete":
      case "onrowenter":
      case "onrowexit":
      case "onrowsinserted":
      case "onstart":
      case "onscroll":
      case "onbeforeeditfocus":
      case "onactivate":
      case "onbeforedeactivate":
      case "ondeactivate":
      case "type":
      case "codebase":
      case "id":
        ret.objAttrs[args[i]] = args[i+1];
        break;
      case "width":
      case "height":
      case "align":
      case "vspace": 
      case "hspace":
      case "class":
      case "title":
      case "accesskey":
      case "name":
      case "tabindex":
        ret.embedAttrs[args[i]] = ret.objAttrs[args[i]] = args[i+1];
        break;
      default:
        ret.embedAttrs[args[i]] = ret.params[args[i]] = args[i+1];
    }
  }
  ret.objAttrs["classid"] = classid;
  if (mimeType) ret.embedAttrs["type"] = mimeType;
  return ret;
}
// p7PMMscripts
/* 

  ================================================
  PVII Pop Menu Magic 2 scripts
  Copyright (c) 2008-2009 Project Seven Development
  www.projectseven.com
  Version:  1.3.5 - script build: 1-31
  ================================================
  
*/

var p7PMMover='_over';
var p7PMMopen='_overdown';
var p7PMMmark='_down';
var p7PMMi=false,p7PMMa=false,p7PMMctl=[],p7PMMadv=[];
function P7_PMMset(){
	var i,h,sh,hd,x,v;
	if(!document.getElementById){
		return;
	}
	sh='.p7PMM div {position:absolute;display:none}\n';
	if(document.styleSheets){
		h='\n<st' + 'yle type="text/css">\n'+sh+'\n</s' + 'tyle>';
		document.write(h);
	}
	else{
		h=document.createElement('style');
		h.type='text/css';
		h.appendChild(document.createTextNode(sh));
		hd=document.getElementsByTagName('head');
		hd[0].appendChild(h);
	}
}
P7_PMMset();
function P7_PMMaddLoad(){
	if(window.addEventListener){
		if(!/KHTML|WebKit/i.test(navigator.userAgent)){
			document.addEventListener("DOMContentLoaded", P7_PMMinit, false);
		}
		window.addEventListener("load",P7_PMMinit,false);
		window.addEventListener("unload",P7_PMMbb,false);
	}
	else if(document.addEventListener){
		document.addEventListener("load",P7_PMMinit,false);
	}
	else if(window.attachEvent){
		document.write("<script id=p7ie_ppm defer src=\"//:\"><\/script>");
		document.getElementById("p7ie_ppm").onreadystatechange=function(){
			if (this.readyState=="complete"){
				if(p7PMMctl.length>0){
					P7_PMMinit();
				}
			}
		};
		window.attachEvent("onload",P7_PMMinit);
	}
	else if(typeof window.onload=='function'){
		var p7loadit=onload;
		window.onload=function(){
			p7loadit();
			P7_PMMinit();
		};
	}
	else{
		window.onload=P7_PMMinit;
	}
}
P7_PMMaddLoad();
function P7_PMMbb(){
	return;
}
function P7_PMMop(){
	if(!document.getElementById){
		return;
	}
	p7PMMctl[p7PMMctl.length]=arguments;
}
function P7_PMMinit(){
	var i,j,jj,k,tM,tA,tU,lv,pp,clv,fs,tS,d=1,cl,tp,uh=0,cN,tD,tN,iM,tw,ow,oh;
	if(p7PMMi){
		return;
	}
	p7PMMi=true;
	document.p7PMMpreload=[];
	for(k=0;k<p7PMMctl.length;k++){
		tM=document.getElementById(p7PMMctl[k][0]);
		if(tM){
			tM.p7opt=p7PMMctl[k];
			if(window.opera||navigator.appVersion.indexOf("MSIE 5")>-1){
				tM.p7opt[2]=0;
			}
			tM.p7PMMtmr=null;
			tD=tM.getElementsByTagName("DIV");
			for(i=0;i<tD.length;i++){
				tD[i].setAttribute("id",tM.id+'d'+(i+2));
				tD[i].p7state='closed';
				tD[i].pmmmenu=tM.id;
			}
			tU=tM.getElementsByTagName("UL");
			for(i=0;i<tU.length;i++){
				tU[i].setAttribute("id",tM.id+'u'+(i+1));
				lv=1;
				pp=tU[i].parentNode;
				while(pp){
					if(pp.id&&pp.id==tM.id){
						break;
					}
					if(pp.tagName&&pp.tagName=="UL"){
						lv++;
					}
					pp=pp.parentNode;
				}
				tU[i].pmmlevel=lv;
				clv='level_'+lv;
				P7_PMMsetClass(tU[i],clv);
				tN=tU[i].childNodes;
				if(tN){
					fs=-1;
					jj=0;
					for(j=0;j<tN.length;j++){
						if(tN[j].tagName&&tN[j].tagName=="LI"){
							jj++;
							tA=tN[j].getElementsByTagName("A")[0];
							if(fs<0){
								P7_PMMsetClass(tA,'pmmfirst');
								P7_PMMsetClass(tN[j],'pmmfirst');
							}
							fs=j;
							tA.setAttribute("id",tM.id+'a'+(d));
							d++;
							tA.pmmlevel=lv;
							tA.pmmdiv=tU[i].parentNode.id;
							tA.pmmmenu=tM.id;
							if(i==0){
								P7_PMMsetClass(tN[j],('root_'+jj));
							}
							tS=tN[j].getElementsByTagName("UL");
							if(tS&&tS.length>0){
								tA.pmmsub=tS[0].parentNode.id;
								if(tM.p7opt[14]&&tM.p7opt[14]==1){
									tA.onmousedown=function(){
										P7_PMMtrigclick(this);
									};
								}
								else{
									tA.onclick=function(){
										return false;
									};
								}
								if(tM.p7opt[13]==1&&lv==1){
									P7_PMMsetClass(tA,'trig_closed_up');
									P7_PMMsetClass(tA.parentNode,'trig_closed_up');
								}
								else if(tM.p7opt[5]==1){
									P7_PMMsetClass(tA,'trig_closed_left');
									P7_PMMsetClass(tA.parentNode,'trig_closed_left');
								}
								else{
									P7_PMMsetClass(tA,'trig_closed');
									P7_PMMsetClass(tA.parentNode,'trig_closed');
								}
							}
							else{
								tA.pmmsub=false;
							}
							tA.onmouseover=function(){
								P7_PMMtrig(this);
							};
							tA.hasImg=false;
							var sr,x,fnA,fnB,swp,s1,s2,s3;
							iM=tA.getElementsByTagName("IMG");
							if(iM&&iM[0]){
								sr=iM[0].getAttribute("src");
								swp=tM.p7opt[9];
								iM[0].pmmswap=swp;
								x=sr.lastIndexOf(".");
								fnA=sr.substring(0,x);
								fnB='.'+sr.substring(x+1);
								s1=fnA+p7PMMover+fnB;
								s2=fnA+p7PMMopen+fnB;
								s3=fnA+p7PMMmark+fnB;
								if(swp==1){
									iM[0].p7imgswap=[sr,s1,s1,s1];
									P7_PMMpreloader(s1);
								}
								else if(swp==2){
									iM[0].p7imgswap=[sr,s1,s2,s2];
									P7_PMMpreloader(s1,s2);
								}
								else if(swp==3){
									iM[0].p7imgswap=[sr,s1,s2,s3];
									P7_PMMpreloader(s1,s2,s3);
								}
								else{
									iM[0].p7imgswap=[sr,sr,sr,sr];
								}
								iM[0].p7state='closed';
								iM[0].mark=false;
								iM[0].rollover=tM.p7opt[10];
								if(swp>0){
									tA.hasImg=true;
									iM[0].onmouseover=function(){
										P7_PMMimovr(this);
									};
									iM[0].onmouseout=function(){
										P7_PMMimout(this);
									};
								}
							}
						}
					}
					if(fs>0){
						P7_PMMsetClass(tA,'pmmlast');
						P7_PMMsetClass(tN[fs],'pmmlast');
					}
				}
			}
			if(tM.p7opt[6]==1){
				tw=tM.offsetWidth;
				oh=tM.offsetHeight;
				ow=P7_PMMgetPropValue(tU[0],'position','position');
				tM.style.height=oh+'px';
				tU[0].style.position="absolute";
				uh=tU[0].offsetWidth;
				tU[0].style.position=ow;
				tM.style.height='auto';
				uh=(uh>=tw)?tw:uh;
				tU[0].style.marginLeft=((tw-uh)/2)+'px';
			}
			tM.onmouseout=function(evt){
				var k,rt,pp,tD,m=true;
				evt=(evt)?evt:((event)?event:null);
				if(evt){
					rt=(evt.relatedTarget)?evt.relatedTarget:evt.toElement;
					if(rt){
						pp=rt.parentNode;
						while(pp){
							if(pp&&pp.id&&pp.id.indexOf(this.id)==0){
								m=false;
								break;
							}
							pp=pp.parentNode;
						}
					}
					if(m){
						if(this.p7PMMtmr){
							clearTimeout(this.p7PMMtmr);
						}
						if(this.p7opt[8]==1){
							this.p7PMMtmr=setTimeout("P7_PMMshut('"+this.id+"')",360);
						}
						else{
							P7_PMMshut(this.id);
						}
					}
				}
			}
			if(tM.p7opt[11]==1){
				P7_PMMcurrentMark(tM);
			}
		}
	}
	p7PMMa=true;
}
function P7_PMMshut(d){
	var k,tM,tD;
	tM=document.getElementById(d);
	tD=tM.getElementsByTagName("DIV");
	for(k=tD.length-1;k>-1;k--){
		if(tD[k].p7state&&tD[k].p7state=='open'){
			P7_PMMclose(tD[k]);
		}
	}
}
function P7_PMMpreloader(){
	var i,x;
	for(i=0;i<arguments.length;i++){
		x=document.p7PMMpreload.length;
		document.p7PMMpreload[x]=new Image();
		document.p7PMMpreload[x].src=arguments[i];
	}
}
function P7_PMMimovr(im){
	var m=false,a=im.parentNode,r=im.rollover;
	if(im.mark){
		m=(r>1)?true:false;
	}
	else if(im.p7state=='open'){
		m=(r==1||r==3)?true:false;
	}
	else{
		m=true;
	}
	if(m){
		im.src=im.p7imgswap[1];
	}
}
function P7_PMMimout(im){
	var a=im.parentNode,r=im.rollover;
	if(im.mark){
		if(im.p7state=='open'){
			im.src=im.p7imgswap[2];
		}
		else{
			im.src=im.p7imgswap[3];
		}
	}
	else if(im.p7state=='open'){
		if(r==1||r==3){
			im.src=im.p7imgswap[2];
		}
	}
	else{
		im.src=im.p7imgswap[0];
	}
}
function P7_PMMtrig(a){
	var dV;
	dV=document.getElementById(a.pmmmenu);
	if(dV.p7PMMtmr){
		clearTimeout(dV.p7PMMtmr);
	}
	if(a.pmmsub){
		if(dV.p7opt[8]==1){
			dV.p7PMMtmr=setTimeout("P7_PMMopen('"+a.id+"')",160);
		}
		else{
			P7_PMMopen(a.id);
		}
	}
	else{
		if(dV.p7opt[8]==1){
			dV.p7PMMtmr=setTimeout("P7_PMMtoggle('"+a.id+"')",160);
		}
		else{
			P7_PMMtoggle(a.id);
		}
	}
}
function P7_PMMopen(d,bp){
	var i,tA,tM,tD,w,h,mT,mL,pp,xx=0,yy=0,wd,pb,pr,sct,aw,sc,pt,pl,iM;
	tA=document.getElementById(d);
	if(!bp){
		P7_PMMtoggle(d);
	}
	if(tA.p7state=='open'){
		return;
	}
	tA.p7state='open';
	if(tA.hasImg){
		iM=tA.getElementsByTagName("IMG")[0];
		iM.p7state='open';
		iM.src=iM.p7imgswap[2];
	}
	tA.className=tA.className.replace('trig_closed','trig_open');
	tA.parentNode.className=tA.parentNode.className.replace('trig_closed','trig_open');
	tM=document.getElementById(tA.pmmmenu);
	tD=document.getElementById(tA.pmmsub);
	tD.p7trigger=tA.id;
	tD.p7state='open';
	tD.style.visibility="hidden";
	tD.style.display="block";
	tD.style.height='auto';
	tD.style.width='auto';
	w=tD.offsetWidth;
	h=tD.offsetHeight;
	aw=tA.parentNode.offsetWidth;
	mT=(tA.parentNode.offsetHeight*-1)+tM.p7opt[3];
	if(tM.p7opt[5]==1){
		mL=(w*-1)-tM.p7opt[4];
	}
	else{
		mL=tA.offsetWidth+tM.p7opt[4];
	}
	if(tM.p7opt[1]==1&&tA.pmmlevel==1){
		mT=0;
		mL=0;
		if(tM.p7opt[7]==1){
			mL=((w-aw)/2)*-1;
		}
		else if(tM.p7opt[7]==2){
			if(tA.className.indexOf('pmmlast')>-1){
				mL=(w-aw)*-1;
			}
			else if(tA.className.indexOf('pmmfirst')==-1){
				mL=((w-aw)/2)*-1;
			}
			else{
				mL=0;
			}
		}
	}
	if(tM.p7opt[13]==1){
		mT-=h;
		if(tM.p7opt[1]==1&&tA.pmmlevel==1){
			mT-=tA.offsetHeight;
		}
	}
	wd=P7_PMMwin();
	pp=tA;
	while(pp){
		xx+=(pp.offsetLeft)?pp.offsetLeft:0;
		yy+=(pp.offsetTop)?pp.offsetTop:0;
		if(window.opera){
			if(pp.nodeName=="BODY"){
				yy-=(pp.offsetTop)?pp.offsetTop:0;
			}
		}
		pp=pp.offsetParent;
	}
	sc=document.body.parentNode.scrollTop;
	if(!sc){
		sc=document.body.scrollTop;
		if(!sc){
			sc=window.scrollY?window.scrollY:0;
		}
	}
	yy-=sc;
	sc=document.body.parentNode.scrollLeft;
	if(!sc){
		sc=document.body.scrollLeft;
		if(sc){
			sc=window.scrollX?window.scrollX:0;
		}
	}
	xx-=sc;
	pr=xx+mL+w;
	pb=yy+mT+h+tA.offsetHeight;
	if(pr>wd[1]){
		if(tM.p7opt[1]==1&&tA.pmmlevel==1){
			mL=mL-(pr-wd[1]+2);
		}
		else{
			mL=w*-1;
		}
	}
	if(pb>wd[0]){
		mT=mT-(pb-wd[0]+2);
	}
	pt=yy+mT+tA.offsetHeight;
	if(pt<0){
		mT=mT-pt+2;
	}
	pl=xx+mL;
	if(pl<0){
		mL=mL-pl+2;
	}
	tD.style.marginTop=mT+'px';
	tD.style.marginLeft=mL+'px';
	if(p7PMMa&&tM.p7opt[2]>0){
		var dfs,fsA,tU=tD.getElementsByTagName('UL')[0];
		fsA=tU.getElementsByTagName("A")[0];
		if(tM.p7opt[2]==1){
			P7_PMManimA(tU.id,5);
		}
		else if(tM.p7opt[2]==2){
			dfs=P7_PMMgetPropValue(fsA,'lineHeight','line-height');
			dfs=(dfs)?dfs:'normal';
			P7_PMManimB(tU.id,5,dfs);
		}
		else if(tM.p7opt[2]==3){
			if(tM.p7opt[1]==1&&tU.pmmlevel==2){
				dfs=P7_PMMgetPropValue(fsA,'lineHeight','line-height');
				dfs=(dfs)?dfs:'normal';
				P7_PMManimB(tU.id,5,dfs);
			}
			else{
				P7_PMManimA(tU.id,5);
			}
		}
		else if(tM.p7opt[2]==4){
			P7_PMManimC(tU.id,-100);
		}
	}
	else{
		tD.style.visibility="visible";
	}
}
function P7_PMMtoggle(d,bp){
	var i,a,tA,tD,pp;
	a=document.getElementById(d);
	pp=a.parentNode;
	while(pp){
		if(pp.tagName&&pp.tagName=="UL"){
			break;
		}
		pp=pp.parentNode;
	}
	tD=pp.getElementsByTagName("DIV");
	if(tD&&tD.length>0){
		for(i=tD.length-1;i>-1;i--){
			if(tD[i].p7state&&tD[i].p7state=='open'){
				if(!bp&&tD[i].p7trigger!=a.id){
					P7_PMMclose(tD[i]);
				}
			}
		}
	}
}
function P7_PMMclose(d,bp){
	var a,h,w,tM;
	a=document.getElementById(d.p7trigger);
	a.p7state='closed';
	d.p7state='closed';
	a.className=a.className.replace('trig_open','trig_closed');
	a.parentNode.className=a.parentNode.className.replace('trig_open','trig_closed');
	if(a.hasImg){
		iM=a.getElementsByTagName("IMG")[0];
		iM.p7state='closed';
		if(iM.mark){
			iM.src=iM.p7imgswap[3];
		}
		else{
			iM.src=iM.p7imgswap[0];
		}
	}
	d.style.display='none';
}
function P7_PMMtrigclick(a){
	var i,h,sB,tA,sh;
	h=a.getAttribute("href");
	if(h.charAt(h.length-1)=='#'||h.search(/javas/i)==0){
		sB=document.getElementById(a.pmmsub);
		tA=sB.getElementsByTagName('A');
		for(i=0;i<tA.length;i++){
			sh=tA[i].getAttribute('href');
			if(sh.charAt(sh.length-1)!='#'&&sh.search(/javas/i)!=0){
				a.setAttribute('href',sh);
				break;
			}
		}
	}
}
function P7_PMMmark(){
	p7PMMadv[p7PMMadv.length]=arguments;
}
function P7_PMMcurrentMark(el){
	var j,i,k,wH,cm=false,mt=['',1,'',''],op,r1,k,kk,tA,aU,pp,a,im;;
	wH=window.location.href;
	if(el.p7opt[12!=1]){
		wH=wH.replace(window.location.search,'');
	}
	if(wH.charAt(wH.length-1)=='#'){
		wH=wH.substring(0,wH.length-1);
	}
	for(k=0;k<p7PMMadv.length;k++){
		if(p7PMMadv[k][0]&&p7PMMadv[k][0]==el.id){
			mt=p7PMMadv[k];
			cm=true;
			break;
		}
	}
	op=mt[1];
	if(op<1){
		return;
	}
	r1=/index\.[\S]*/i;
	k=-1,kk=-1;
	tA=el.getElementsByTagName("A");
	for(j=0;j<tA.length;j++){
		aU=tA[j].href.replace(r1,'');
		if(op>0){
			if(tA[j].href==wH||aU==wH){
				k=j;
				kk=-1;
			}
		}
		if(op==2){
			if(tA[j].firstChild){
				if(tA[j].firstChild.nodeValue==mt[2]){
					kk=j;
				}
			}
		}
		if(op==3&&tA[j].href.indexOf(mt[2])>-1){
			kk=j;
		}
		if(op==4){
			for(x=2;x<mt.length;x+=2){
				if(wH.indexOf(mt[x])>-1){
					if(tA[j].firstChild&&tA[j].firstChild.nodeValue){
						if(tA[j].firstChild.nodeValue==mt[x+1]){
							kk=j;
						}
					}
				}
			}
		}
	}
	k=(kk>k)?kk:k;
	if(k>-1){
		pp=tA[k].parentNode;
		while(pp){
			if(pp.tagName&&pp.tagName=='LI'){
				P7_PMMsetClass(pp,'li_current_mark');
				a=pp.getElementsByTagName('A');
				if(a&&a[0]){
					P7_PMMsetClass(a[0],'current_mark');
					if(a[0].hasImg){
						im=a[0].getElementsByTagName('IMG')[0];
						im.mark=true;
						im.src=im.p7imgswap[3];
					}
				}
			}
			else{
				if(pp==el){
					break;
				}
			}
			pp=pp.parentNode;
		}
	}
}
function P7_PMManimA(id,v,ft){
	var el,tg=100,fr=10,dy=10;
	el=document.getElementById(id);
	el.style.fontSize=v+'%';
	if(!ft){
		el.style.visibility="visible";
		el.parentNode.style.visibility="visible";
	}
	if(v!=tg){
		v+=fr;
		v=(v>=tg)?tg:v;
		setTimeout("P7_PMManimA('"+id+"',"+v+",1)",dy);
	}
}
function P7_PMManimB(id,v,ln,ft){
	var el,tg=120,fr=10,dy=10;
	el=document.getElementById(id);
	el.style.lineHeight=v+'%';
	if(!ft){
		el.style.visibility="visible";
		el.parentNode.style.visibility="visible";
	}
	if(v!=tg){
		v+=fr;
		v=(v>=tg)?tg:v;
		setTimeout("P7_PMManimB('"+id+"',"+v+",'"+ln+"',1)",dy);
	}
	else{
		el.style.lineHeight=ln;
	}
}
function P7_PMManimC(id,v,ft){
	var el,tg=0,fr=8,dy=10;
	el=document.getElementById(id);
	el.style.textIndent=v+'px';
	if(!ft){
		el.style.visibility="visible";
		el.parentNode.style.visibility="visible";
	}
	if(v!=tg){
		v+=fr;
		v=(v>=tg)?tg:v;
		setTimeout("P7_PMManimC('"+id+"',"+v+",1)",dy);
	}
}
function P7_PMMsetClass(ob,cl){
	var cc,nc,r=/\s+/g;
	cc=ob.className;
	nc=cl;
	if(cc&&cc.length>0){
		nc=cc+' '+cl;
	}
	nc=nc.replace(r,' ');
	ob.className=nc;
}
function P7_PMMremClass(ob,cl){
	var cc,nc,r=/\s+/g;;
	cc=ob.className;
	if(cc&&cc.indexOf(cl>-1)){
		nc=cc.replace(cl,'');
		nc=nc.replace(r,' ');
		ob.className=nc;
	}
}
function P7_PMMwin(){
	var h,w;
	if(document.documentElement&&document.documentElement.clientHeight){
		w=document.documentElement.clientWidth;
		h=document.documentElement.clientHeight;
	}
	else if(window.innerHeight){
		if(document.documentElement.clientWidth){
			w=document.documentElement.clientWidth;
		}
		else{
			w=window.innerWidth;
		}
		h=window.innerHeight;
	}
	else if(document.body){
		w=document.body.clientWidth;
		h=document.body.clientHeight;
	}
	return [h,w];
}
function P7_PMMgetPropValue(ob,prop,prop2){
	var h,v=null;
	if(ob){
		if(ob.currentStyle){
			v=eval('ob.currentStyle.'+prop);
		}
		else if(document.defaultView.getComputedStyle(ob,"")){
			v=document.defaultView.getComputedStyle(ob,"").getPropertyValue(prop2);
		}
		else{
			v=eval("ob.style."+prop);
		}
	}
	return v;
}
// SpryMenuBar

// Copyright (c) 2006. Adobe Systems Incorporated.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of Adobe Systems Incorporated nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/*******************************************************************************

 SpryMenuBar.js
 This file handles the JavaScript for Spry Menu Bar.  You should have no need
 to edit this file.  Some highlights of the MenuBar object is that timers are
 used to keep submenus from showing up until the user has hovered over the parent
 menu item for some time, as well as a timer for when they leave a submenu to keep
 showing that submenu until the timer fires.

 *******************************************************************************/

var Spry;
if(!Spry)
{
	Spry = {};
}
if(!Spry.Widget)
{
	Spry.Widget = {};
}

// Constructor for Menu Bar
// element should be an ID of an unordered list (<ul> tag)
// preloadImage1 and preloadImage2 are images for the rollover state of a menu
Spry.Widget.MenuBar = function(element, opts)
{
	this.init(element, opts);
};

Spry.Widget.MenuBar.prototype.init = function(element, opts)
{
	this.element = this.getElement(element);

	// represents the current (sub)menu we are operating on
	this.currMenu = null;

	var isie = (typeof document.all != 'undefined' && typeof window.opera == 'undefined' && navigator.vendor != 'KDE');
	if(typeof document.getElementById == 'undefined' || (navigator.vendor == 'Apple Computer, Inc.' && typeof window.XMLHttpRequest == 'undefined') || (isie && typeof document.uniqueID == 'undefined'))
	{
		// bail on older unsupported browsers
		return;
	}

	// load hover images now
	if(opts)
	{
		for(var k in opts)
		{
			var rollover = new Image;
			rollover.src = opts[k];
		}
	}

	if(this.element)
	{
		this.currMenu = this.element;
		var items = this.element.getElementsByTagName('li');
		for(var i=0; i<items.length; i++)
		{
			this.initialize(items[i], element, isie);
			if(isie)
			{
				this.addClassName(items[i], "MenuBarItemIE");
				items[i].style.position = "static";
			}
		}
		if(isie)
		{
			if(this.hasClassName(this.element, "MenuBarVertical"))
			{
				this.element.style.position = "relative";
			}
			var linkitems = this.element.getElementsByTagName('a');
			for(var i=0; i<linkitems.length; i++)
			{
				linkitems[i].style.position = "relative";
			}
		}
	}
};

Spry.Widget.MenuBar.prototype.getElement = function(ele)
{
	if (ele && typeof ele == "string")
		return document.getElementById(ele);
	return ele;
};

Spry.Widget.MenuBar.prototype.hasClassName = function(ele, className)
{
	if (!ele || !className || !ele.className || ele.className.search(new RegExp("\\b" + className + "\\b")) == -1)
	{
		return false;
	}
	return true;
};

Spry.Widget.MenuBar.prototype.addClassName = function(ele, className)
{
	if (!ele || !className || this.hasClassName(ele, className))
		return;
	ele.className += (ele.className ? " " : "") + className;
};

Spry.Widget.MenuBar.prototype.removeClassName = function(ele, className)
{
	if (!ele || !className || !this.hasClassName(ele, className))
		return;
	ele.className = ele.className.replace(new RegExp("\\s*\\b" + className + "\\b", "g"), "");
};

// addEventListener for Menu Bar
// attach an event to a tag without creating obtrusive HTML code
Spry.Widget.MenuBar.prototype.addEventListener = function(element, eventType, handler, capture)
{
	try
	{
		if (element.addEventListener)
		{
			element.addEventListener(eventType, handler, capture);
		}
		else if (element.attachEvent)
		{
			element.attachEvent('on' + eventType, handler);
		}
	}
	catch (e) {}
};

// createIframeLayer for Menu Bar
// creates an IFRAME underneath a menu so that it will show above form controls and ActiveX
Spry.Widget.MenuBar.prototype.createIframeLayer = function(menu)
{
	var layer = document.createElement('iframe');
	layer.tabIndex = '-1';
	layer.src = 'javascript:false;';
	menu.parentNode.appendChild(layer);
	
	layer.style.left = menu.offsetLeft + 'px';
	layer.style.top = menu.offsetTop + 'px';
	layer.style.width = menu.offsetWidth + 'px';
	layer.style.height = menu.offsetHeight + 'px';
};

// removeIframeLayer for Menu Bar
// removes an IFRAME underneath a menu to reveal any form controls and ActiveX
Spry.Widget.MenuBar.prototype.removeIframeLayer =  function(menu)
{
	var layers = menu.parentNode.getElementsByTagName('iframe');
	while(layers.length > 0)
	{
		layers[0].parentNode.removeChild(layers[0]);
	}
};

// clearMenus for Menu Bar
// root is the top level unordered list (<ul> tag)
Spry.Widget.MenuBar.prototype.clearMenus = function(root)
{
	var menus = root.getElementsByTagName('ul');
	for(var i=0; i<menus.length; i++)
	{
		this.hideSubmenu(menus[i]);
	}
	this.removeClassName(this.element, "MenuBarActive");
};

// bubbledTextEvent for Menu Bar
// identify bubbled up text events in Safari so we can ignore them
Spry.Widget.MenuBar.prototype.bubbledTextEvent = function()
{
	return (navigator.vendor == 'Apple Computer, Inc.' && (event.target == event.relatedTarget.parentNode || (event.eventPhase == 3 && event.target.parentNode == event.relatedTarget)));
};

// showSubmenu for Menu Bar
// set the proper CSS class on this menu to show it
Spry.Widget.MenuBar.prototype.showSubmenu = function(menu)
{
	if(this.currMenu)
	{
		this.clearMenus(this.currMenu);
		this.currMenu = null;
	}
	
	if(menu)
	{
		this.addClassName(menu, "MenuBarSubmenuVisible");
		if(typeof document.all != 'undefined' && typeof window.opera == 'undefined' && navigator.vendor != 'KDE')
		{
			if(!this.hasClassName(this.element, "MenuBarHorizontal") || menu.parentNode.parentNode != this.element)
			{
				menu.style.top = menu.parentNode.offsetTop + 'px';
			}
		}
		if(typeof document.uniqueID != "undefined")
		{
			this.createIframeLayer(menu);
		}
	}
	this.addClassName(this.element, "MenuBarActive");
};

// hideSubmenu for Menu Bar
// remove the proper CSS class on this menu to hide it
Spry.Widget.MenuBar.prototype.hideSubmenu = function(menu)
{
	if(menu)
	{
		this.removeClassName(menu, "MenuBarSubmenuVisible");
		if(typeof document.all != 'undefined' && typeof window.opera == 'undefined' && navigator.vendor != 'KDE')
		{
			menu.style.top = '';
			menu.style.left = '';
		}
		this.removeIframeLayer(menu);
	}
};

// initialize for Menu Bar
// create event listeners for the Menu Bar widget so we can properly
// show and hide submenus
Spry.Widget.MenuBar.prototype.initialize = function(listitem, element, isie)
{
	var opentime, closetime;
	var link = listitem.getElementsByTagName('a')[0];
	var submenus = listitem.getElementsByTagName('ul');
	var menu = (submenus.length > 0 ? submenus[0] : null);

	var hasSubMenu = false;
	if(menu)
	{
		this.addClassName(link, "MenuBarItemSubmenu");
		hasSubMenu = true;
	}

	if(!isie)
	{
		// define a simple function that comes standard in IE to determine
		// if a node is within another node
		listitem.contains = function(testNode)
		{
			// this refers to the list item
			if(testNode == null)
			{
				return false;
			}
			if(testNode == this)
			{
				return true;
			}
			else
			{
				return this.contains(testNode.parentNode);
			}
		};
	}
	
	// need to save this for scope further down
	var self = this;

	this.addEventListener(listitem, 'mouseover', function(e)
	{
		if(self.bubbledTextEvent())
		{
			// ignore bubbled text events
			return;
		}
		clearTimeout(closetime);
		if(self.currMenu == listitem)
		{
			self.currMenu = null;
		}
		// show menu highlighting
		self.addClassName(link, hasSubMenu ? "MenuBarItemSubmenuHover" : "MenuBarItemHover");
		if(menu && !self.hasClassName(menu, "MenuBarSubmenuVisible"))
		{
			opentime = window.setTimeout(function(){self.showSubmenu(menu);}, 250);
		}
	}, false);

	this.addEventListener(listitem, 'mouseout', function(e)
	{
		if(self.bubbledTextEvent())
		{
			// ignore bubbled text events
			return;
		}

		var related = (typeof e.relatedTarget != 'undefined' ? e.relatedTarget : e.toElement);
		if(!listitem.contains(related))
		{
			clearTimeout(opentime);
			self.currMenu = listitem;

			// remove menu highlighting
			self.removeClassName(link, hasSubMenu ? "MenuBarItemSubmenuHover" : "MenuBarItemHover");
			if(menu)
			{
				closetime = window.setTimeout(function(){self.hideSubmenu(menu);}, 600);
			}
		}
	}, false);
};
// StreetSmart (MyPage)
function preventMultiFrames(){
// prevent pages from being within iframes

var thisF= this.window
var allF = parent.top.frames;
for(var i=0;i<allF.length;i++) {
  if(allF[i]==thisF){
    var frameName=allF[i].name; // this is the variable you are looking for
    if (frameName == "ss_frame") {
      top.location.href = document.location.href ;
    }
  }
}
}

function removeForumCookie(){
        expires = new Date();
        expires.setMinutes( expires.getMinutes() + 5 );

  document.cookie = "forum_k=; expires=" + expires.toGMTString() + "; path=/;domain=.orientalmotor.com";
  document.cookie = "forum_sid=; expires=" + expires.toGMTString() + "; path=/;domain=.orientalmotor.com";
  document.cookie = "new=; expires=" + expires.toGMTString() + "; path=/;domain=.orientalmotor.com";
}
// Universal Search
function PartNum() {
	document.getElementById('UniversalKeywordSearch').style.display = "none";
	document.getElementById('UniversalSpecSearch').style.display = "none";
	document.getElementById('UniversalPartNumberSearch').style.display = "block";
	document.getElementById('PartSearchTop').style.backgroundColor = "#B9D3F3";
	document.getElementById('KeywordSearchTop').style.backgroundColor = "#E3E3E3";
	document.getElementById('SpecSearchTop').style.backgroundColor = "#E3E3E3";
}
function KeySearch() {
	document.getElementById('UniversalPartNumberSearch').style.display = "none";
	document.getElementById('UniversalSpecSearch').style.display = "none";
	document.getElementById('UniversalKeywordSearch').style.display = "block";
	document.getElementById('PartSearchTop').style.backgroundColor = "#E3E3E3";
	document.getElementById('KeywordSearchTop').style.backgroundColor = "#B9D3F3";
	document.getElementById('SpecSearchTop').style.backgroundColor = "#E3E3E3";
}
function AdvSearch() {
	document.getElementById('UniversalPartNumberSearch').style.display = "none";
	document.getElementById('UniversalKeywordSearch').style.display = "none";
	document.getElementById('UniversalSpecSearch').style.display = "block";
	document.getElementById('PartSearchTop').style.backgroundColor = "#E3E3E3";
	document.getElementById('KeywordSearchTop').style.backgroundColor = "#E3E3E3";
	document.getElementById('SpecSearchTop').style.backgroundColor = "#B9D3F3";
}

//CatNav Search

function ctlSearch_OnClick ()
{
 var url;
 var theform = document.frmSearchBox;
 
 if (theform.keyword.value.length < 2)
 {
 }
 else
 {
	var iIndex = theform.ddlSearchType.selectedIndex;
	var sSearchType = theform.ddlSearchType.options[iIndex].value;

  
// Version 2.7 release
// 1. using default Domain, like "orientalmotor.thomasnet.com" in our sample 
// 2. using root category shortName, like "all-categories" in our sample
  url = "http://catalog.orientalmotor.com/keyword/?&plpver=11&key=all&keycateg=100"
  url = url + "&SchType=" + sSearchType;
  url = url + "&keyword=" + encodeURI(theform.keyword.value);
  url = url + "&refer=" + encodeURI("http://" + document.location.hostname);
   
  document.location.href = url;
 }

//alert (url);				
}
function MM_jumpMenu(targ,selObj,restore){ //v3.0
  eval(targ+".location='"+selObj.options[selObj.selectedIndex].value+"'");
  if (restore) selObj.selectedIndex=0;
}
//Tabs for LM
  	function TabStart() {
	document.getElementById('TabsDefault').style.display = "none";
  	document.getElementById('TabsMenuI').style.display = "block";
  	document.getElementById('TabsMenuII').style.display = "none";
	document.getElementById('TabContentII').style.display = "none";
  	document.getElementById('TabsMenuIII').style.display = "none";
	document.getElementById('TabContentIII').style.display = "none";
   	document.getElementById('TabsMenuIV').style.display = "none";
	document.getElementById('TabContentIV').style.display = "none";
  	document.getElementById('TabsMenuV').style.display = "none";
	document.getElementById('TabContentV').style.display = "none";
  	document.getElementById('TabsMenuVI').style.display = "none";
	document.getElementById('TabContentVI').style.display = "none";
  	document.getElementById('TabsMenuVII').style.display = "none";
	document.getElementById('TabContentVII').style.display = "none";
	}
	function TabI() {
  	document.getElementById('TabsMenuI').style.display = "block";
	document.getElementById('TabContentI').style.display = "block";
  	document.getElementById('TabsMenuII').style.display = "none";
	document.getElementById('TabContentII').style.display = "none";
  	document.getElementById('TabsMenuIII').style.display = "none";
	document.getElementById('TabContentIII').style.display = "none";
   	document.getElementById('TabsMenuIV').style.display = "none";
	document.getElementById('TabContentIV').style.display = "none";
  	document.getElementById('TabsMenuV').style.display = "none";
	document.getElementById('TabContentV').style.display = "none";
  	document.getElementById('TabsMenuVI').style.display = "none";
	document.getElementById('TabContentVI').style.display = "none";
  	document.getElementById('TabsMenuVII').style.display = "none";
	document.getElementById('TabContentVII').style.display = "none";
	document.cookie="tab" + "=" + "default" + "; expires=" + exp.toGMTString() + ";path=/" + ";domain=.orientalmotor.com";
}
   function TabII() {
  	document.getElementById('TabsMenuI').style.display = "none";
	document.getElementById('TabContentI').style.display = "none";
  	document.getElementById('TabsMenuII').style.display = "block";
	document.getElementById('TabContentII').style.display = "block";
  	document.getElementById('TabsMenuIII').style.display = "none";
	document.getElementById('TabContentIII').style.display = "none";
   	document.getElementById('TabsMenuIV').style.display = "none";
	document.getElementById('TabContentIV').style.display = "none";
  	document.getElementById('TabsMenuV').style.display = "none";
	document.getElementById('TabContentV').style.display = "none";
  	document.getElementById('TabsMenuVI').style.display = "none";
	document.getElementById('TabContentVI').style.display = "none";
  	document.getElementById('TabsMenuVII').style.display = "none";
	document.getElementById('TabContentVII').style.display = "none";
	document.cookie="tab" + "=" + "default" + "; expires=" + exp.toGMTString() + ";path=/" + ";domain=.orientalmotor.com";
	}
   function TabIII() {
  	document.getElementById('TabsMenuI').style.display = "none";
	document.getElementById('TabContentI').style.display = "none";
  	document.getElementById('TabsMenuII').style.display = "none";
	document.getElementById('TabContentII').style.display = "none";
  	document.getElementById('TabsMenuIII').style.display = "block";
	document.getElementById('TabContentIII').style.display = "block";
   	document.getElementById('TabsMenuIV').style.display = "none";
	document.getElementById('TabContentIV').style.display = "none";
  	document.getElementById('TabsMenuV').style.display = "none";
	document.getElementById('TabContentV').style.display = "none";
  	document.getElementById('TabsMenuVI').style.display = "none";
	document.getElementById('TabContentVI').style.display = "none";
  	document.getElementById('TabsMenuVII').style.display = "none";
	document.getElementById('TabContentVII').style.display = "none";
	document.cookie="tab" + "=" + "default" + "; expires=" + exp.toGMTString() + ";path=/" + ";domain=.orientalmotor.com";
 }
   function TabIV() {
  	document.getElementById('TabsMenuI').style.display = "none";
	document.getElementById('TabContentI').style.display = "none";
  	document.getElementById('TabsMenuII').style.display = "none";
	document.getElementById('TabContentII').style.display = "none";
  	document.getElementById('TabsMenuIII').style.display = "none";
	document.getElementById('TabContentIII').style.display = "none";
   	document.getElementById('TabsMenuIV').style.display = "block";
	document.getElementById('TabContentIV').style.display = "block";
  	document.getElementById('TabsMenuV').style.display = "none";
	document.getElementById('TabContentV').style.display = "none";
  	document.getElementById('TabsMenuVI').style.display = "none";
	document.getElementById('TabContentVI').style.display = "none";
  	document.getElementById('TabsMenuVII').style.display = "none";
	document.getElementById('TabContentVII').style.display = "none";
	document.cookie="tab" + "=" + "default" + "; expires=" + exp.toGMTString() + ";path=/" + ";domain=.orientalmotor.com";
 }
   function TabV() {
  	document.getElementById('TabsMenuI').style.display = "none";
	document.getElementById('TabContentI').style.display = "none";
  	document.getElementById('TabsMenuII').style.display = "none";
	document.getElementById('TabContentII').style.display = "none";
  	document.getElementById('TabsMenuIII').style.display = "none";
	document.getElementById('TabContentIII').style.display = "none";
   	document.getElementById('TabsMenuIV').style.display = "none";
	document.getElementById('TabContentIV').style.display = "none";
  	document.getElementById('TabsMenuV').style.display = "block";
	document.getElementById('TabContentV').style.display = "block";
  	document.getElementById('TabsMenuVI').style.display = "none";
	document.getElementById('TabContentVI').style.display = "none";
  	document.getElementById('TabsMenuVII').style.display = "none";
	document.getElementById('TabContentVII').style.display = "none";
	document.cookie="tab" + "=" + "default" + "; expires=" + exp.toGMTString() + ";path=/" + ";domain=.orientalmotor.com";
 }
   function TabVI() {
  	document.getElementById('TabsMenuI').style.display = "none";
	document.getElementById('TabContentI').style.display = "none";
  	document.getElementById('TabsMenuII').style.display = "none";
	document.getElementById('TabContentII').style.display = "none";
  	document.getElementById('TabsMenuIII').style.display = "none";
	document.getElementById('TabContentIII').style.display = "none";
   	document.getElementById('TabsMenuIV').style.display = "none";
	document.getElementById('TabContentIV').style.display = "none";
  	document.getElementById('TabsMenuV').style.display = "none";
	document.getElementById('TabContentV').style.display = "none";
  	document.getElementById('TabsMenuVI').style.display = "block";
	document.getElementById('TabContentVI').style.display = "block";
  	document.getElementById('TabsMenuVII').style.display = "none";
	document.getElementById('TabContentVII').style.display = "none";
	document.cookie="tab" + "=" + "default" + "; expires=" + exp.toGMTString() + ";path=/" + ";domain=.orientalmotor.com";
 }
   function TabVII() {
  	document.getElementById('TabsMenuI').style.display = "none";
	document.getElementById('TabContentI').style.display = "none";
  	document.getElementById('TabsMenuII').style.display = "none";
	document.getElementById('TabContentII').style.display = "none";
  	document.getElementById('TabsMenuIII').style.display = "none";
	document.getElementById('TabContentIII').style.display = "none";
   	document.getElementById('TabsMenuIV').style.display = "none";
	document.getElementById('TabContentIV').style.display = "none";
  	document.getElementById('TabsMenuV').style.display = "none";
	document.getElementById('TabContentV').style.display = "none";
  	document.getElementById('TabsMenuVI').style.display = "none";
	document.getElementById('TabContentVI').style.display = "none";
  	document.getElementById('TabsMenuVII').style.display = "block";
	document.getElementById('TabContentVII').style.display = "block";
	document.cookie="tab" + "=" + "default" + "; expires=" + exp.toGMTString() + ";path=/" + ";domain=.orientalmotor.com";
 }
// Save Tab Code - Set Cookies
	function setLineupCookie(name,value,expiredays)
	{
	var exdate=new Date();
	exdate.setDate(exdate.getDate()+1);
	document.cookie="tab" + "=" + "lineup" + ";expires=Thu, 1 Jan 2099 00:00:01 UTC" + ";path=/" + ";domain=.orientalmotor.com"; 
	}
	function setTorqueCookie(name,value,expiredays)
	{
	var exdate=new Date();
	exdate.setDate(exdate.getDate()+1);
	document.cookie="tab" + "=" + "torque" + ";expires=Thu, 1 Jan 2099 00:00:01 UTC" + ";path=/" + ";domain=.orientalmotor.com"; 
}
