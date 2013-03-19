
function setLeftForTable(g,f,j){for(var i=2;i<=f;i++){var h="table_"+g+"_"+i;document.getElementById(h).style.left=(i-1)*j+"px";document.getElementById(h).style.width=j+"px";}}
function Slider(i,h,n,m,l,k){this.sId=i;this.isItem=k;this.iItemWidth=h;this.iItemPerPage=parseInt(n);if(k=="true"){this.iPageWidth=document.getElementById("table_"+i+"_1").offsetWidth;var j=this.getPageCount();setLeftForTable(i,j,this.iPageWidth);}else{this.iPageWidth=h*n;}
this.iOffset=m;this.iSlideInterval=l;this.busy=false;}
function getSliderForItem(f){var e="key"+f;if(aSliders[e]){return aSliders[e];}
var h=document.getElementById("itemCount"+f).value;var g=new Slider(f,165,h,100,90,"true");aSliders[e]=g;return aSliders[e];}
function changeHREF(d){var c=d.getAttribute("href");d.setAttribute("href",d.getAttribute("url"));d.setAttribute("url",c);d.setAttribute("onmouseover","");}
function onSurvey(f){var e=Math.round((screen.height-800)/2);var h=Math.round((screen.width-800)/2);var g=["location=no","menubar=no","status=no","resizable=yes","scrollbars=yes"];g=g.concat("top="+e,"left="+h,"width=800","height=800");window.open(f,"Survey",g.join(","));}
var aSliders=new Array;Slider.prototype.slide=function(j,i,p){if(this.busy){return;}
if(!this.isSlidable(p)){return;}
this.busy=true;var o=this.getLeftArrow();var n=this.getRightArrow();var m=this.getCurrentPage();var l=p==0?m-1:m+1;var k=this.getPageCount();if(l==1){o.className="arrow inactive";o.style.cursor="default";}else{if(p!=0){o.className="arrow clickable";o.style.cursor="pointer";}}
if(l==k){n.className="arrow inactive";n.style.cursor="default";}else{if(p!=1){n.className="arrow clickable";n.style.cursor="pointer";}}
this.loadImages(l);this.doSlide(this.iPageWidth,this.iOffset,p);if(this.isItem=="true"&&l!=1){document.getElementById("table_"+j+"_"+l).className="remtablem";}};Slider.prototype.doSlide=function(r,q,p){if(r<=0){this.busy=false;return;}
var o=this.getItemListDiv();var n=r-q>0?q:r;n=p==0?n:-n;r=r-q;var m=o.style.left;var l=m.length>0?parseInt(m,10):0;o.style.left=l+n+"px";var k=this;var j=function(){k.doSlide(r,q,p);};setTimeout(j,this.iSlideInterval);};Slider.prototype.isSlidable=function(e){var d=this.getCurrentPage();var f=this.getPageCount();if(e==0&&d>1){return true;}else{if(e==1&&d<f){return true;}}
return false;};Slider.prototype.hover=function(d,c){if(!d){return;}
if(d.className.indexOf("clickable")>-1){d.className="arrow hover";d.style.cursor="pointer";}};Slider.prototype.leave=function(d,c){if(!d){return;}
if(d.className.indexOf("hover")>-1){d.className="arrow clickable";d.style.cursor="pointer";}};Slider.prototype.getCurrentPage=function(){var f=this.getItemListDiv();var e=f.style.left;var h=e.length>0?parseInt(e,10):0;h=Math.abs(h);var g=parseInt(h/this.iPageWidth)+1;return g;};Slider.prototype.getPageCount=function(){var e=this.getItemListDiv();var d=e.children.length;if(this.isItem=="true"){var f=d;}else{var f=d%this.iItemPerPage==0?d/this.iItemPerPage:parseInt(d/this.iItemPerPage)+1;}
return f;};Slider.prototype.loadImages=function(g){var f=this.getItemListTd();if(!f||f.length==0){return;}
var j=(g-1)*this.iItemPerPage;var i=j+this.iItemPerPage>=f.length?f.length:j+this.iItemPerPage;for(var h=j;h<i;h++){this.createImageElementIfNecessary(f[h]);}};Slider.prototype.createImageElementIfNecessary=function(f){var e=f.getAttribute("value");if(e!=null){var h=this.getImageParent(f);var g=h.getElementsByTagName("img");g[0].src=e;}};Slider.prototype.getImageParent=function(e){if(!e){return null;}
var d=e.getElementsByTagName("span");if(!d||d.length==0){return null;}
for(var f=0;f<d.length;f++){if(d[f].className.indexOf("rvi")!=-1){return d[0];}}
return null;};Slider.prototype.getContentDiv=function(){var d="fs-content-"+this.sId;var c=document.getElementById(d);return c;};Slider.prototype.getItemListDiv=function(){var d="fs-itemList-"+this.sId;var c=document.getElementById(d);return c;};Slider.prototype.getItemListTd=function(){var e="fs-itemList-"+this.sId;var d=document.getElementById(e);var f=d.getElementsByTagName("td");return f;};Slider.prototype.getLeftArrow=function(){var d="fs-leftArrow-"+this.sId;var c=document.getElementById(d);return c;};Slider.prototype.getRightArrow=function(){var d="fs-rightArrow-"+this.sId;var c=document.getElementById(d);return c;};function redirect(b){var a=new Image();b=b+"&redirect=false";a.src=b;return true;}
