(function( _function ) {
  //Private Method
  function getJson(url) {
		let rq = fetch(url);
		let dataJs;
		rq.then(res=>{
			res.json().then(data=>{
				dataJs = data;
			})
		})
		return dataJs;
	}
  //Public Method
	_function.getEleByID = (id) => {
		return document.getElementById(id);
	}
	_function.getJson = (url)=>{
		return getJson(url);
	}
	_function.toggleDisplay = (id, type)=>{
		document.getElementById(id).classList.toggle(type);
	}

	_function.hasClass = (id, className)=>{
		if(_function.getEleByID(id).className.indexOf(className) != -1) return true;
		return false;
	}

	_function.randInt = (min, max)=>{
		return Math.floor(Math.random()*(max - min + 1) + min);
	}
	
}( window._function = window._function || {}));

(function( _menu ) {
  //Private Property
  var isSlide = false;

  //Private Method
  function slideToggleMenu(e) {
		let id = e.target.id;

		if(id == 'menu-btn'){
			if(!isSlide)
				_function.getEleByID('main-menu').style.display = "block";
			else 
				_function.getEleByID('main-menu').style.display = "";
			isSlide = !isSlide;
		}
	}

	//Public Method
  _menu.init = ()=>{
  	_function.getEleByID('menu-btn').addEventListener("click", slideToggleMenu);
  }
  _menu.init();
}( window._menu = window._menu || {}));


document.addEventListener("click", (e)=>{
	var id = e.target.id;
	if(id.indexOf("selectAPId") != -1){
		scan.selectAP(parseInt(id.split("").reverse().join("")));
	}
	if(id.indexOf("selectSTId") != -1){
		scan.selectST(parseInt(id.split("").reverse().join("")));
	}
})