var ZepDoc = (function($){

    var ZepDoc = {};


    ZepDoc.drawLeftMenu = function(search){

        if(search){

        }else{
            var fullData = {
                "classes"    : ZephirApi.classes,
                "namespaces" : ZephirApi.namespaces
            };
        }

        $("#menu-wrapper").empty();
        $("#menu-wrapper").append(ZepDoc.drawMenuNode(fullData));


    };

    ZepDoc.drawMenuNode = function(localData){

        var $node = $("<ul/>");
        $node.addClass("menu-node");


        for(var i = 0 ; i<localData.namespaces.length ; i++){

            var curNs = ZephirApi.allNamespaces[localData.namespaces[i]];
            var $nsItem = $("<li class='menu-ns'><a></a></li>");
            $nsItem.find("a").html(curNs.shortName);
            $nsItem.attr("data-ns-name",curNs.name);

            $nsItem.append(ZepDoc.drawMenuNode(curNs));
            $node.append($nsItem);
        }

        for(var i = 0 ; i<localData.classes.length ; i++){

            var curClass = ZephirApi.allClasses[localData.classes[i]];
            var $cItem = $("<li class='menu-class'><a></a></li>");
            $cItem.find("a").html(curClass.shortname);

            $cItem.find("a").attr("href", ZepCurrentPath  + "class/" + curClass.name.replace("\\","/") + ".html");
            $cItem.addClass( "type-" + curClass.type);
            $cItem.attr("data-class-name",curClass.name);


            $node.append($cItem);

        }

        return $node;


    };




    $(function(){

        ZepDoc.drawLeftMenu();

    });

    return ZepDoc;
})(jQuery);