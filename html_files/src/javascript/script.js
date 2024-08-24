document.getElementById('open_btn').addEventListener('click', function () {
    document.getElementById('sidebar').classList.toggle('open-sidebar');
});

/// Busca o elemento com a classe `box`
var box = document.getElementById('sidebar_content');
/// Busca os elementos quem tem a tag `A` que estão dentro do elemento `box`
var meusLinks = box.querySelectorAll('A');
    
/// Para cada `A` adiciona o evento de onclick
meusLinks.forEach(function( a ){ 
   a.addEventListener('click', onCliqueiNoLink);
});


/// Evento de onclick
function onCliqueiNoLink(){
  
  /// `this` dentro dessa função é a tag `A` que foi clicada.
  // console.log( this , this.parentNode );

  /// `this` dentro dessa função é a tag `A` que foi clicada.
  Array.from(document.getElementsByClassName('side-item')).forEach( function( a ){
      a.classList.remove('active');
  });

  this.parentNode.classList.add('active');

}