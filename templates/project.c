


<!DOCTYPE html>
<html>
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# githubog: http://ogp.me/ns/fb/githubog#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title>zephir/templates/project.c at 4f26d50aaf36d58f44f04c122a5a29cf2b47af39 · phalcon/zephir · GitHub</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png" />
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png" />
    <link rel="logo" type="image/svg" href="https://github-media-downloads.s3.amazonaws.com/github-logo.svg" />
    <meta property="og:image" content="https://github.global.ssl.fastly.net/images/modules/logos_page/Octocat.png">
    <meta name="hostname" content="github-fe127-cp1-prd.iad.github.net">
    <meta name="ruby" content="ruby 1.9.3p194-tcs-github-tcmalloc (2012-05-25, TCS patched 2012-05-27, GitHub v1.0.36) [x86_64-linux]">
    <link rel="assets" href="https://github.global.ssl.fastly.net/">
    <link rel="conduit-xhr" href="https://ghconduit.com:25035/">
    <link rel="xhr-socket" href="/_sockets" />
    


    <meta name="msapplication-TileImage" content="/windows-tile.png" />
    <meta name="msapplication-TileColor" content="#ffffff" />
    <meta name="selected-link" value="repo_source" data-pjax-transient />
    <meta content="collector.githubapp.com" name="octolytics-host" /><meta content="github" name="octolytics-app-id" /><meta content="BD86AC10:1FA7:103703C7:525596F2" name="octolytics-dimension-request_id" />
    

    
    
    <link rel="icon" type="image/x-icon" href="/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="d3nEZVUqUDwu+23K/4zDFmFo0Fk8UpZx9owJZm1ZYAM=" name="csrf-token" />

    <link href="https://github.global.ssl.fastly.net/assets/github-a71513c1910860883fdda62fab52b84ac9cbbbc4.css" media="all" rel="stylesheet" type="text/css" />
    <link href="https://github.global.ssl.fastly.net/assets/github2-2c50a5e9876e57298f41ce245404978c1e9cfa79.css" media="all" rel="stylesheet" type="text/css" />
    

    

      <script src="https://github.global.ssl.fastly.net/assets/frameworks-3e962e599763b7c5f9668b8c1f3a968b7ebf72e9.js" type="text/javascript"></script>
      <script src="https://github.global.ssl.fastly.net/assets/github-6d050b70294adc8845bc8d7b9aebf334c6edaf51.js" type="text/javascript"></script>
      
      <meta http-equiv="x-pjax-version" content="0a2c5c198191cbc0497fd56b74c39a35">

        <link data-pjax-transient rel='permalink' href='/phalcon/zephir/blob/4f26d50aaf36d58f44f04c122a5a29cf2b47af39/templates/project.c'>
  <meta property="og:title" content="zephir"/>
  <meta property="og:type" content="githubog:gitrepository"/>
  <meta property="og:url" content="https://github.com/phalcon/zephir"/>
  <meta property="og:image" content="https://github.global.ssl.fastly.net/images/gravatars/gravatar-user-420.png"/>
  <meta property="og:site_name" content="GitHub"/>
  <meta property="og:description" content="zephir - Zephir is a compiled high level language aimed to the creation of C-extensions for PHP"/>

  <meta name="description" content="zephir - Zephir is a compiled high level language aimed to the creation of C-extensions for PHP" />

  <meta content="1221505" name="octolytics-dimension-user_id" /><meta content="phalcon" name="octolytics-dimension-user_login" /><meta content="11478636" name="octolytics-dimension-repository_id" /><meta content="phalcon/zephir" name="octolytics-dimension-repository_nwo" /><meta content="true" name="octolytics-dimension-repository_public" /><meta content="false" name="octolytics-dimension-repository_is_fork" /><meta content="11478636" name="octolytics-dimension-repository_network_root_id" /><meta content="phalcon/zephir" name="octolytics-dimension-repository_network_root_nwo" />
  <link href="https://github.com/phalcon/zephir/commits/4f26d50aaf36d58f44f04c122a5a29cf2b47af39.atom" rel="alternate" title="Recent Commits to zephir:4f26d50aaf36d58f44f04c122a5a29cf2b47af39" type="application/atom+xml" />

  </head>


  <body class="logged_out  env-production  vis-public  page-blob">
    <div class="wrapper">
      
      
      


      
      <div class="header header-logged-out">
  <div class="container clearfix">

    <a class="header-logo-wordmark" href="https://github.com/">
      <span class="mega-octicon octicon-logo-github"></span>
    </a>

    <div class="header-actions">
        <a class="button primary" href="/signup">Sign up</a>
      <a class="button signin" href="/login?return_to=%2Fphalcon%2Fzephir%2Fblob%2F4f26d50aaf36d58f44f04c122a5a29cf2b47af39%2Ftemplates%2Fproject.c">Sign in</a>
    </div>

    <div class="command-bar js-command-bar  in-repository">

      <ul class="top-nav">
          <li class="explore"><a href="/explore">Explore</a></li>
        <li class="features"><a href="/features">Features</a></li>
          <li class="enterprise"><a href="https://enterprise.github.com/">Enterprise</a></li>
          <li class="blog"><a href="/blog">Blog</a></li>
      </ul>
        <form accept-charset="UTF-8" action="/search" class="command-bar-form" id="top_search_form" method="get">

<input type="text" data-hotkey="/ s" name="q" id="js-command-bar-field" placeholder="Search or type a command" tabindex="1" autocapitalize="off"
    
    
      data-repo="phalcon/zephir"
      data-branch="4f26d50aaf36d58f44f04c122a5a29cf2b47af39"
      data-sha="a8a104c65f4042be2d5147dfefb3c25c7e9e3f1d"
  >

    <input type="hidden" name="nwo" value="phalcon/zephir" />

    <div class="select-menu js-menu-container js-select-menu search-context-select-menu">
      <span class="minibutton select-menu-button js-menu-target">
        <span class="js-select-button">This repository</span>
      </span>

      <div class="select-menu-modal-holder js-menu-content js-navigation-container">
        <div class="select-menu-modal">

          <div class="select-menu-item js-navigation-item js-this-repository-navigation-item selected">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" class="js-search-this-repository" name="search_target" value="repository" checked="checked" />
            <div class="select-menu-item-text js-select-button-text">This repository</div>
          </div> <!-- /.select-menu-item -->

          <div class="select-menu-item js-navigation-item js-all-repositories-navigation-item">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" name="search_target" value="global" />
            <div class="select-menu-item-text js-select-button-text">All repositories</div>
          </div> <!-- /.select-menu-item -->

        </div>
      </div>
    </div>

  <span class="octicon help tooltipped downwards" title="Show command bar help">
    <span class="octicon octicon-question"></span>
  </span>


  <input type="hidden" name="ref" value="cmdform">

</form>
    </div>

  </div>
</div>


      


          <div class="site" itemscope itemtype="http://schema.org/WebPage">
    
    <div class="pagehead repohead instapaper_ignore readability-menu">
      <div class="container">
        

<ul class="pagehead-actions">


  <li>
  <a href="/login?return_to=%2Fphalcon%2Fzephir"
  class="minibutton with-count js-toggler-target star-button entice tooltipped upwards"
  title="You must be signed in to use this feature" rel="nofollow">
  <span class="octicon octicon-star"></span>Star
</a>
<a class="social-count js-social-count" href="/phalcon/zephir/stargazers">
  169
</a>

  </li>

    <li>
      <a href="/login?return_to=%2Fphalcon%2Fzephir"
        class="minibutton with-count js-toggler-target fork-button entice tooltipped upwards"
        title="You must be signed in to fork a repository" rel="nofollow">
        <span class="octicon octicon-git-branch"></span>Fork
      </a>
      <a href="/phalcon/zephir/network" class="social-count">
        17
      </a>
    </li>
</ul>

        <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public">
          <span class="repo-label"><span>public</span></span>
          <span class="mega-octicon octicon-repo"></span>
          <span class="author">
            <a href="/phalcon" class="url fn" itemprop="url" rel="author"><span itemprop="title">phalcon</span></a></span
          ><span class="repohead-name-divider">/</span><strong
          ><a href="/phalcon/zephir" class="js-current-repository js-repo-home-link">zephir</a></strong>

          <span class="page-context-loader">
            <img alt="Octocat-spinner-32" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
          </span>

        </h1>
      </div><!-- /.container -->
    </div><!-- /.repohead -->

    <div class="container">

      <div class="repository-with-sidebar repo-container ">

        <div class="repository-sidebar">
            

<div class="repo-nav repo-nav-full js-repository-container-pjax js-octicon-loaders">
  <div class="repo-nav-contents">
    <ul class="repo-menu">
      <li class="tooltipped leftwards" title="Code">
        <a href="/phalcon/zephir" aria-label="Code" class="js-selected-navigation-item selected" data-gotokey="c" data-pjax="true" data-selected-links="repo_source repo_downloads repo_commits repo_tags repo_branches /phalcon/zephir">
          <span class="octicon octicon-code"></span> <span class="full-word">Code</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


      <li class="tooltipped leftwards" title="Pull Requests"><a href="/phalcon/zephir/pulls" aria-label="Pull Requests" class="js-selected-navigation-item js-disable-pjax" data-gotokey="p" data-selected-links="repo_pulls /phalcon/zephir/pulls">
            <span class="octicon octicon-git-pull-request"></span> <span class="full-word">Pull Requests</span>
            <span class='counter'>0</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


        <li class="tooltipped leftwards" title="Wiki">
          <a href="/phalcon/zephir/wiki" aria-label="Wiki" class="js-selected-navigation-item " data-pjax="true" data-selected-links="repo_wiki /phalcon/zephir/wiki">
            <span class="octicon octicon-book"></span> <span class="full-word">Wiki</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>        </li>
    </ul>
    <div class="repo-menu-separator"></div>
    <ul class="repo-menu">

      <li class="tooltipped leftwards" title="Pulse">
        <a href="/phalcon/zephir/pulse" aria-label="Pulse" class="js-selected-navigation-item " data-pjax="true" data-selected-links="pulse /phalcon/zephir/pulse">
          <span class="octicon octicon-pulse"></span> <span class="full-word">Pulse</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped leftwards" title="Graphs">
        <a href="/phalcon/zephir/graphs" aria-label="Graphs" class="js-selected-navigation-item " data-pjax="true" data-selected-links="repo_graphs repo_contributors /phalcon/zephir/graphs">
          <span class="octicon octicon-graph"></span> <span class="full-word">Graphs</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped leftwards" title="Network">
        <a href="/phalcon/zephir/network" aria-label="Network" class="js-selected-navigation-item js-disable-pjax" data-selected-links="repo_network /phalcon/zephir/network">
          <span class="octicon octicon-git-branch"></span> <span class="full-word">Network</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>
    </ul>


  </div>
</div>

            <div class="only-with-full-nav">
              

  

<div class="clone-url open"
  data-protocol-type="http"
  data-url="/users/set_protocol?protocol_selector=http&amp;protocol_type=clone">
  <h3><strong>HTTPS</strong> clone URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/phalcon/zephir.git" readonly="readonly">

    <span class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/phalcon/zephir.git" data-copied-hint="copied!" title="copy to clipboard"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>

  

<div class="clone-url "
  data-protocol-type="subversion"
  data-url="/users/set_protocol?protocol_selector=subversion&amp;protocol_type=clone">
  <h3><strong>Subversion</strong> checkout URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/phalcon/zephir" readonly="readonly">

    <span class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/phalcon/zephir" data-copied-hint="copied!" title="copy to clipboard"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>


<p class="clone-options">You can clone with
      <a href="#" class="js-clone-selector" data-protocol="http">HTTPS</a>,
      or <a href="#" class="js-clone-selector" data-protocol="subversion">Subversion</a>.
  <span class="octicon help tooltipped upwards" title="Get help on which URL is right for you.">
    <a href="https://help.github.com/articles/which-remote-url-should-i-use">
    <span class="octicon octicon-question"></span>
    </a>
  </span>
</p>



              <a href="/phalcon/zephir/archive/4f26d50aaf36d58f44f04c122a5a29cf2b47af39.zip"
                 class="minibutton sidebar-button"
                 title="Download this repository as a zip file"
                 rel="nofollow">
                <span class="octicon octicon-cloud-download"></span>
                Download ZIP
              </a>
            </div>
        </div><!-- /.repository-sidebar -->

        <div id="js-repo-pjax-container" class="repository-content context-loader-container" data-pjax-container>
          


<!-- blob contrib key: blob_contributors:v21:345b428a42d77e1fb7fe9fdf0b414047 -->

<p title="This is a placeholder element" class="js-history-link-replace hidden"></p>

<a href="/phalcon/zephir/find/4f26d50aaf36d58f44f04c122a5a29cf2b47af39" data-pjax data-hotkey="t" class="js-show-file-finder" style="display:none">Show File Finder</a>

<div class="file-navigation">
  
  

<div class="select-menu js-menu-container js-select-menu" >
  <span class="minibutton select-menu-button js-menu-target" data-hotkey="w"
    data-master-branch="master"
    data-ref=""
    role="button" aria-label="Switch branches or tags" tabindex="0">
    <span class="octicon octicon-git-branch"></span>
    <i>tree:</i>
    <span class="js-select-button">4f26d50aaf</span>
  </span>

  <div class="select-menu-modal-holder js-menu-content js-navigation-container" data-pjax>

    <div class="select-menu-modal">
      <div class="select-menu-header">
        <span class="select-menu-title">Switch branches/tags</span>
        <span class="octicon octicon-remove-close js-menu-close"></span>
      </div> <!-- /.select-menu-header -->

      <div class="select-menu-filters">
        <div class="select-menu-text-filter">
          <input type="text" aria-label="Filter branches/tags" id="context-commitish-filter-field" class="js-filterable-field js-navigation-enable" placeholder="Filter branches/tags">
        </div>
        <div class="select-menu-tabs">
          <ul>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="branches" class="js-select-menu-tab">Branches</a>
            </li>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="tags" class="js-select-menu-tab">Tags</a>
            </li>
          </ul>
        </div><!-- /.select-menu-tabs -->
      </div><!-- /.select-menu-filters -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="branches">

        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/phalcon/zephir/blob/master/templates/project.c"
                 data-name="master"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="master">master</a>
            </div> <!-- /.select-menu-item -->
        </div>

          <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="tags">
        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


        </div>

        <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

    </div> <!-- /.select-menu-modal -->
  </div> <!-- /.select-menu-modal-holder -->
</div> <!-- /.select-menu -->

  <div class="breadcrumb">
    <span class='repo-root js-repo-root'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/phalcon/zephir/tree/4f26d50aaf36d58f44f04c122a5a29cf2b47af39" data-branch="4f26d50aaf36d58f44f04c122a5a29cf2b47af39" data-direction="back" data-pjax="true" itemscope="url" rel="nofollow"><span itemprop="title">zephir</span></a></span></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/phalcon/zephir/tree/4f26d50aaf36d58f44f04c122a5a29cf2b47af39/templates" data-branch="4f26d50aaf36d58f44f04c122a5a29cf2b47af39" data-direction="back" data-pjax="true" itemscope="url" rel="nofollow"><span itemprop="title">templates</span></a></span><span class="separator"> / </span><strong class="final-path">project.c</strong> <span class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="templates/project.c" data-copied-hint="copied!" title="copy to clipboard"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>


  <div class="commit commit-loader file-history-tease js-deferred-content" data-url="/phalcon/zephir/contributors/4f26d50aaf36d58f44f04c122a5a29cf2b47af39/templates/project.c">
    Fetching contributors…

    <div class="participation">
      <p class="loader-loading"><img alt="Octocat-spinner-32-eaf2f5" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32-EAF2F5.gif" width="16" /></p>
      <p class="loader-error">Cannot retrieve contributors at this time</p>
    </div>
  </div>

<div id="files" class="bubble">
  <div class="file">
    <div class="meta">
      <div class="info">
        <span class="icon"><b class="octicon octicon-file-text"></b></span>
        <span class="mode" title="File Mode">file</span>
          <span>138 lines (106 sloc)</span>
        <span>3.433 kb</span>
      </div>
      <div class="actions">
        <div class="button-group">
              <a class="minibutton disabled js-entice" href=""
                 data-entice="You must be signed in to make or propose changes">Edit</a>
          <a href="/phalcon/zephir/raw/4f26d50aaf36d58f44f04c122a5a29cf2b47af39/templates/project.c" class="button minibutton " id="raw-url">Raw</a>
            <a href="/phalcon/zephir/blame/4f26d50aaf36d58f44f04c122a5a29cf2b47af39/templates/project.c" class="button minibutton ">Blame</a>
          <a href="/phalcon/zephir/commits/4f26d50aaf36d58f44f04c122a5a29cf2b47af39/templates/project.c" class="button minibutton " rel="nofollow">History</a>
        </div><!-- /.button-group -->
          <a class="minibutton danger empty-icon js-entice" href=""
             data-entice="You must be signed in and on a branch to make or propose changes">
          Delete
        </a>
      </div><!-- /.actions -->

    </div>
        <div class="blob-wrapper data type-c js-blob-data">
        <table class="file-code file-diff">
          <tr class="file-code-line">
            <td class="blob-line-nums">
              <span id="L1" rel="#L1">1</span>
<span id="L2" rel="#L2">2</span>
<span id="L3" rel="#L3">3</span>
<span id="L4" rel="#L4">4</span>
<span id="L5" rel="#L5">5</span>
<span id="L6" rel="#L6">6</span>
<span id="L7" rel="#L7">7</span>
<span id="L8" rel="#L8">8</span>
<span id="L9" rel="#L9">9</span>
<span id="L10" rel="#L10">10</span>
<span id="L11" rel="#L11">11</span>
<span id="L12" rel="#L12">12</span>
<span id="L13" rel="#L13">13</span>
<span id="L14" rel="#L14">14</span>
<span id="L15" rel="#L15">15</span>
<span id="L16" rel="#L16">16</span>
<span id="L17" rel="#L17">17</span>
<span id="L18" rel="#L18">18</span>
<span id="L19" rel="#L19">19</span>
<span id="L20" rel="#L20">20</span>
<span id="L21" rel="#L21">21</span>
<span id="L22" rel="#L22">22</span>
<span id="L23" rel="#L23">23</span>
<span id="L24" rel="#L24">24</span>
<span id="L25" rel="#L25">25</span>
<span id="L26" rel="#L26">26</span>
<span id="L27" rel="#L27">27</span>
<span id="L28" rel="#L28">28</span>
<span id="L29" rel="#L29">29</span>
<span id="L30" rel="#L30">30</span>
<span id="L31" rel="#L31">31</span>
<span id="L32" rel="#L32">32</span>
<span id="L33" rel="#L33">33</span>
<span id="L34" rel="#L34">34</span>
<span id="L35" rel="#L35">35</span>
<span id="L36" rel="#L36">36</span>
<span id="L37" rel="#L37">37</span>
<span id="L38" rel="#L38">38</span>
<span id="L39" rel="#L39">39</span>
<span id="L40" rel="#L40">40</span>
<span id="L41" rel="#L41">41</span>
<span id="L42" rel="#L42">42</span>
<span id="L43" rel="#L43">43</span>
<span id="L44" rel="#L44">44</span>
<span id="L45" rel="#L45">45</span>
<span id="L46" rel="#L46">46</span>
<span id="L47" rel="#L47">47</span>
<span id="L48" rel="#L48">48</span>
<span id="L49" rel="#L49">49</span>
<span id="L50" rel="#L50">50</span>
<span id="L51" rel="#L51">51</span>
<span id="L52" rel="#L52">52</span>
<span id="L53" rel="#L53">53</span>
<span id="L54" rel="#L54">54</span>
<span id="L55" rel="#L55">55</span>
<span id="L56" rel="#L56">56</span>
<span id="L57" rel="#L57">57</span>
<span id="L58" rel="#L58">58</span>
<span id="L59" rel="#L59">59</span>
<span id="L60" rel="#L60">60</span>
<span id="L61" rel="#L61">61</span>
<span id="L62" rel="#L62">62</span>
<span id="L63" rel="#L63">63</span>
<span id="L64" rel="#L64">64</span>
<span id="L65" rel="#L65">65</span>
<span id="L66" rel="#L66">66</span>
<span id="L67" rel="#L67">67</span>
<span id="L68" rel="#L68">68</span>
<span id="L69" rel="#L69">69</span>
<span id="L70" rel="#L70">70</span>
<span id="L71" rel="#L71">71</span>
<span id="L72" rel="#L72">72</span>
<span id="L73" rel="#L73">73</span>
<span id="L74" rel="#L74">74</span>
<span id="L75" rel="#L75">75</span>
<span id="L76" rel="#L76">76</span>
<span id="L77" rel="#L77">77</span>
<span id="L78" rel="#L78">78</span>
<span id="L79" rel="#L79">79</span>
<span id="L80" rel="#L80">80</span>
<span id="L81" rel="#L81">81</span>
<span id="L82" rel="#L82">82</span>
<span id="L83" rel="#L83">83</span>
<span id="L84" rel="#L84">84</span>
<span id="L85" rel="#L85">85</span>
<span id="L86" rel="#L86">86</span>
<span id="L87" rel="#L87">87</span>
<span id="L88" rel="#L88">88</span>
<span id="L89" rel="#L89">89</span>
<span id="L90" rel="#L90">90</span>
<span id="L91" rel="#L91">91</span>
<span id="L92" rel="#L92">92</span>
<span id="L93" rel="#L93">93</span>
<span id="L94" rel="#L94">94</span>
<span id="L95" rel="#L95">95</span>
<span id="L96" rel="#L96">96</span>
<span id="L97" rel="#L97">97</span>
<span id="L98" rel="#L98">98</span>
<span id="L99" rel="#L99">99</span>
<span id="L100" rel="#L100">100</span>
<span id="L101" rel="#L101">101</span>
<span id="L102" rel="#L102">102</span>
<span id="L103" rel="#L103">103</span>
<span id="L104" rel="#L104">104</span>
<span id="L105" rel="#L105">105</span>
<span id="L106" rel="#L106">106</span>
<span id="L107" rel="#L107">107</span>
<span id="L108" rel="#L108">108</span>
<span id="L109" rel="#L109">109</span>
<span id="L110" rel="#L110">110</span>
<span id="L111" rel="#L111">111</span>
<span id="L112" rel="#L112">112</span>
<span id="L113" rel="#L113">113</span>
<span id="L114" rel="#L114">114</span>
<span id="L115" rel="#L115">115</span>
<span id="L116" rel="#L116">116</span>
<span id="L117" rel="#L117">117</span>
<span id="L118" rel="#L118">118</span>
<span id="L119" rel="#L119">119</span>
<span id="L120" rel="#L120">120</span>
<span id="L121" rel="#L121">121</span>
<span id="L122" rel="#L122">122</span>
<span id="L123" rel="#L123">123</span>
<span id="L124" rel="#L124">124</span>
<span id="L125" rel="#L125">125</span>
<span id="L126" rel="#L126">126</span>
<span id="L127" rel="#L127">127</span>
<span id="L128" rel="#L128">128</span>
<span id="L129" rel="#L129">129</span>
<span id="L130" rel="#L130">130</span>
<span id="L131" rel="#L131">131</span>
<span id="L132" rel="#L132">132</span>
<span id="L133" rel="#L133">133</span>
<span id="L134" rel="#L134">134</span>
<span id="L135" rel="#L135">135</span>
<span id="L136" rel="#L136">136</span>
<span id="L137" rel="#L137">137</span>

            </td>
            <td class="blob-line-code">
                    <div class="highlight"><pre><div class='line' id='LC1'><br/></div><div class='line' id='LC2'><span class="cp">#ifdef HAVE_CONFIG_H</span></div><div class='line' id='LC3'><span class="cp">#include &quot;config.h&quot;</span></div><div class='line' id='LC4'><span class="cp">#endif</span></div><div class='line' id='LC5'><br/></div><div class='line' id='LC6'><span class="cp">#include &lt;php.h&gt;</span></div><div class='line' id='LC7'><span class="cp">#include &quot;php_ext.h&quot;</span></div><div class='line' id='LC8'><span class="cp">#include &quot;%PROJECT_LOWER%.h&quot;</span></div><div class='line' id='LC9'><br/></div><div class='line' id='LC10'><span class="cp">#include &lt;ext/standard/info.h&gt;</span></div><div class='line' id='LC11'><br/></div><div class='line' id='LC12'><span class="cp">#include &lt;Zend/zend_operators.h&gt;</span></div><div class='line' id='LC13'><span class="cp">#include &lt;Zend/zend_exceptions.h&gt;</span></div><div class='line' id='LC14'><span class="cp">#include &lt;Zend/zend_interfaces.h&gt;</span></div><div class='line' id='LC15'><br/></div><div class='line' id='LC16'><span class="cp">#include &quot;kernel/main.h&quot;</span></div><div class='line' id='LC17'><span class="cp">#include &quot;kernel/memory.h&quot;</span></div><div class='line' id='LC18'><br/></div><div class='line' id='LC19'><span class="o">%</span><span class="n">CLASS_ENTRIES</span><span class="o">%</span></div><div class='line' id='LC20'><br/></div><div class='line' id='LC21'><span class="n">ZEND_DECLARE_MODULE_GLOBALS</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">)</span></div><div class='line' id='LC22'><br/></div><div class='line' id='LC23'><span class="n">PHP_MINIT_FUNCTION</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">){</span></div><div class='line' id='LC24'><br/></div><div class='line' id='LC25'>	<span class="o">%</span><span class="n">CLASS_INITS</span><span class="o">%</span></div><div class='line' id='LC26'>	<span class="k">return</span> <span class="n">SUCCESS</span><span class="p">;</span></div><div class='line' id='LC27'><span class="p">}</span></div><div class='line' id='LC28'><br/></div><div class='line' id='LC29'><span class="cp">#ifndef ZEPHIR_RELEASE</span></div><div class='line' id='LC30'><span class="k">static</span> <span class="n">PHP_MSHUTDOWN_FUNCTION</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">){</span></div><div class='line' id='LC31'><br/></div><div class='line' id='LC32'>	<span class="n">assert</span><span class="p">(</span><span class="n">ZEPHIR_GLOBAL</span><span class="p">(</span><span class="n">function_cache</span><span class="p">)</span> <span class="o">==</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC33'>	<span class="c1">//assert(ZEPHIR_GLOBAL(orm).parser_cache == NULL);</span></div><div class='line' id='LC34'>	<span class="c1">//assert(ZEPHIR_GLOBAL(orm).ast_cache == NULL);</span></div><div class='line' id='LC35'><br/></div><div class='line' id='LC36'>	<span class="k">return</span> <span class="n">SUCCESS</span><span class="p">;</span></div><div class='line' id='LC37'><span class="p">}</span></div><div class='line' id='LC38'><span class="cp">#endif</span></div><div class='line' id='LC39'><br/></div><div class='line' id='LC40'><span class="k">static</span> <span class="n">PHP_RINIT_FUNCTION</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">){</span></div><div class='line' id='LC41'><br/></div><div class='line' id='LC42'>	<span class="n">php_zephir_init_globals</span><span class="p">(</span><span class="n">ZEPHIR_VGLOBAL</span> <span class="n">TSRMLS_CC</span><span class="p">);</span></div><div class='line' id='LC43'>	<span class="c1">//%PROJECT_LOWER%_init_interned_strings(TSRMLS_C);</span></div><div class='line' id='LC44'><br/></div><div class='line' id='LC45'>	<span class="k">return</span> <span class="n">SUCCESS</span><span class="p">;</span></div><div class='line' id='LC46'><span class="p">}</span></div><div class='line' id='LC47'><br/></div><div class='line' id='LC48'><span class="k">static</span> <span class="n">PHP_RSHUTDOWN_FUNCTION</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">){</span></div><div class='line' id='LC49'><br/></div><div class='line' id='LC50'>	<span class="k">if</span> <span class="p">(</span><span class="n">ZEPHIR_GLOBAL</span><span class="p">(</span><span class="n">start_memory</span><span class="p">)</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC51'>		<span class="n">zephir_clean_restore_stack</span><span class="p">(</span><span class="n">TSRMLS_C</span><span class="p">);</span></div><div class='line' id='LC52'>	<span class="p">}</span></div><div class='line' id='LC53'><br/></div><div class='line' id='LC54'>	<span class="k">if</span> <span class="p">(</span><span class="n">ZEPHIR_GLOBAL</span><span class="p">(</span><span class="n">function_cache</span><span class="p">)</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">)</span> <span class="p">{</span></div><div class='line' id='LC55'>		<span class="n">zend_hash_destroy</span><span class="p">(</span><span class="n">ZEPHIR_GLOBAL</span><span class="p">(</span><span class="n">function_cache</span><span class="p">));</span></div><div class='line' id='LC56'>		<span class="n">FREE_HASHTABLE</span><span class="p">(</span><span class="n">ZEPHIR_GLOBAL</span><span class="p">(</span><span class="n">function_cache</span><span class="p">));</span></div><div class='line' id='LC57'>		<span class="n">ZEPHIR_GLOBAL</span><span class="p">(</span><span class="n">function_cache</span><span class="p">)</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC58'>	<span class="p">}</span></div><div class='line' id='LC59'><br/></div><div class='line' id='LC60'>	<span class="k">return</span> <span class="n">SUCCESS</span><span class="p">;</span></div><div class='line' id='LC61'><span class="p">}</span></div><div class='line' id='LC62'><br/></div><div class='line' id='LC63'><span class="k">static</span> <span class="n">PHP_MINFO_FUNCTION</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">)</span></div><div class='line' id='LC64'><span class="p">{</span></div><div class='line' id='LC65'>	<span class="n">php_info_print_table_start</span><span class="p">();</span></div><div class='line' id='LC66'>	<span class="n">php_info_print_table_row</span><span class="p">(</span><span class="mi">2</span><span class="p">,</span> <span class="s">&quot;Version&quot;</span><span class="p">,</span> <span class="n">PHP_</span><span class="o">%</span><span class="n">PROJECT_UPPER</span><span class="o">%</span><span class="n">_VERSION</span><span class="p">);</span></div><div class='line' id='LC67'>	<span class="n">php_info_print_table_end</span><span class="p">();</span></div><div class='line' id='LC68'><span class="p">}</span></div><div class='line' id='LC69'><br/></div><div class='line' id='LC70'><span class="k">static</span> <span class="n">PHP_GINIT_FUNCTION</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">)</span></div><div class='line' id='LC71'><span class="p">{</span></div><div class='line' id='LC72'>	<span class="n">zephir_memory_entry</span> <span class="o">*</span><span class="n">start</span><span class="p">;</span></div><div class='line' id='LC73'><br/></div><div class='line' id='LC74'>	<span class="n">php_zephir_init_globals</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span> <span class="n">TSRMLS_CC</span><span class="p">);</span></div><div class='line' id='LC75'><br/></div><div class='line' id='LC76'>	<span class="cm">/* Start Memory Frame */</span></div><div class='line' id='LC77'>	<span class="n">start</span> <span class="o">=</span> <span class="p">(</span><span class="n">zephir_memory_entry</span> <span class="o">*</span><span class="p">)</span> <span class="n">pecalloc</span><span class="p">(</span><span class="mi">1</span><span class="p">,</span> <span class="k">sizeof</span><span class="p">(</span><span class="n">zephir_memory_entry</span><span class="p">),</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC78'>	<span class="n">start</span><span class="o">-&gt;</span><span class="n">addresses</span>       <span class="o">=</span> <span class="n">pecalloc</span><span class="p">(</span><span class="mi">24</span><span class="p">,</span> <span class="k">sizeof</span><span class="p">(</span><span class="n">zval</span><span class="o">*</span><span class="p">),</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC79'>	<span class="n">start</span><span class="o">-&gt;</span><span class="n">capacity</span>        <span class="o">=</span> <span class="mi">24</span><span class="p">;</span></div><div class='line' id='LC80'>	<span class="n">start</span><span class="o">-&gt;</span><span class="n">hash_addresses</span>  <span class="o">=</span> <span class="n">pecalloc</span><span class="p">(</span><span class="mi">8</span><span class="p">,</span> <span class="k">sizeof</span><span class="p">(</span><span class="n">zval</span><span class="o">*</span><span class="p">),</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC81'>	<span class="n">start</span><span class="o">-&gt;</span><span class="n">hash_capacity</span>   <span class="o">=</span> <span class="mi">8</span><span class="p">;</span></div><div class='line' id='LC82'><br/></div><div class='line' id='LC83'>	<span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">start_memory</span> <span class="o">=</span> <span class="n">start</span><span class="p">;</span></div><div class='line' id='LC84'><br/></div><div class='line' id='LC85'>	<span class="cm">/* Global Constants */</span></div><div class='line' id='LC86'>	<span class="n">ALLOC_PERMANENT_ZVAL</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_false</span><span class="p">);</span></div><div class='line' id='LC87'>	<span class="n">INIT_PZVAL</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_false</span><span class="p">);</span></div><div class='line' id='LC88'>	<span class="n">ZVAL_FALSE</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_false</span><span class="p">);</span></div><div class='line' id='LC89'>	<span class="n">Z_ADDREF_P</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_false</span><span class="p">);</span></div><div class='line' id='LC90'><br/></div><div class='line' id='LC91'>	<span class="n">ALLOC_PERMANENT_ZVAL</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_true</span><span class="p">);</span></div><div class='line' id='LC92'>	<span class="n">INIT_PZVAL</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_true</span><span class="p">);</span></div><div class='line' id='LC93'>	<span class="n">ZVAL_TRUE</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_true</span><span class="p">);</span></div><div class='line' id='LC94'>	<span class="n">Z_ADDREF_P</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_true</span><span class="p">);</span></div><div class='line' id='LC95'><br/></div><div class='line' id='LC96'>	<span class="n">ALLOC_PERMANENT_ZVAL</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_null</span><span class="p">);</span></div><div class='line' id='LC97'>	<span class="n">INIT_PZVAL</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_null</span><span class="p">);</span></div><div class='line' id='LC98'>	<span class="n">ZVAL_NULL</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_null</span><span class="p">);</span></div><div class='line' id='LC99'>	<span class="n">Z_ADDREF_P</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">global_null</span><span class="p">);</span></div><div class='line' id='LC100'><span class="p">}</span></div><div class='line' id='LC101'><br/></div><div class='line' id='LC102'><span class="k">static</span> <span class="n">PHP_GSHUTDOWN_FUNCTION</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">)</span></div><div class='line' id='LC103'><span class="p">{</span></div><div class='line' id='LC104'>	<span class="n">assert</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">start_memory</span> <span class="o">!=</span> <span class="nb">NULL</span><span class="p">);</span></div><div class='line' id='LC105'><br/></div><div class='line' id='LC106'>	<span class="n">pefree</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">start_memory</span><span class="o">-&gt;</span><span class="n">hash_addresses</span><span class="p">,</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC107'>	<span class="n">pefree</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">start_memory</span><span class="o">-&gt;</span><span class="n">addresses</span><span class="p">,</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC108'>	<span class="n">pefree</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">start_memory</span><span class="p">,</span> <span class="mi">1</span><span class="p">);</span></div><div class='line' id='LC109'>	<span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_globals</span><span class="o">-&gt;</span><span class="n">start_memory</span> <span class="o">=</span> <span class="nb">NULL</span><span class="p">;</span></div><div class='line' id='LC110'><span class="p">}</span></div><div class='line' id='LC111'><br/></div><div class='line' id='LC112'><span class="n">zend_module_entry</span> <span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="n">_module_entry</span> <span class="o">=</span> <span class="p">{</span></div><div class='line' id='LC113'>	<span class="n">STANDARD_MODULE_HEADER_EX</span><span class="p">,</span></div><div class='line' id='LC114'>	<span class="nb">NULL</span><span class="p">,</span></div><div class='line' id='LC115'>	<span class="nb">NULL</span><span class="p">,</span></div><div class='line' id='LC116'>	<span class="n">PHP_</span><span class="o">%</span><span class="n">PROJECT_UPPER</span><span class="o">%</span><span class="n">_EXTNAME</span><span class="p">,</span></div><div class='line' id='LC117'>	<span class="nb">NULL</span><span class="p">,</span></div><div class='line' id='LC118'>	<span class="n">PHP_MINIT</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC119'><span class="cp">#ifndef ZEPHIR_RELEASE</span></div><div class='line' id='LC120'>	<span class="n">PHP_MSHUTDOWN</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC121'><span class="cp">#else</span></div><div class='line' id='LC122'>	<span class="nb">NULL</span><span class="p">,</span></div><div class='line' id='LC123'><span class="cp">#endif</span></div><div class='line' id='LC124'>	<span class="n">PHP_RINIT</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC125'>	<span class="n">PHP_RSHUTDOWN</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC126'>	<span class="n">PHP_MINFO</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC127'>	<span class="n">PHP_</span><span class="o">%</span><span class="n">PROJECT_UPPER</span><span class="o">%</span><span class="n">_VERSION</span><span class="p">,</span></div><div class='line' id='LC128'>	<span class="n">ZEND_MODULE_GLOBALS</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC129'>	<span class="n">PHP_GINIT</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC130'>	<span class="n">PHP_GSHUTDOWN</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">),</span></div><div class='line' id='LC131'>	<span class="nb">NULL</span><span class="p">,</span></div><div class='line' id='LC132'>	<span class="n">STANDARD_MODULE_PROPERTIES_EX</span></div><div class='line' id='LC133'><span class="p">};</span></div><div class='line' id='LC134'><br/></div><div class='line' id='LC135'><span class="cp">#ifdef COMPILE_DL_%PROJECT_UPPER%</span></div><div class='line' id='LC136'><span class="n">ZEND_GET_MODULE</span><span class="p">(</span><span class="o">%</span><span class="n">PROJECT_LOWER</span><span class="o">%</span><span class="p">)</span></div><div class='line' id='LC137'><span class="cp">#endif</span></div></pre></div>
            </td>
          </tr>
        </table>
  </div>

  </div>
</div>

<a href="#jump-to-line" rel="facebox[.linejump]" data-hotkey="l" class="js-jump-to-line" style="display:none">Jump to Line</a>
<div id="jump-to-line" style="display:none">
  <form accept-charset="UTF-8" class="js-jump-to-line-form">
    <input class="linejump-input js-jump-to-line-field" type="text" placeholder="Jump to line&hellip;" autofocus>
    <button type="submit" class="button">Go</button>
  </form>
</div>

        </div>

      </div><!-- /.repo-container -->
      <div class="modal-backdrop"></div>
    </div><!-- /.container -->
  </div><!-- /.site -->


    </div><!-- /.wrapper -->

      <div class="container">
  <div class="site-footer">
    <ul class="site-footer-links right">
      <li><a href="https://status.github.com/">Status</a></li>
      <li><a href="http://developer.github.com">API</a></li>
      <li><a href="http://training.github.com">Training</a></li>
      <li><a href="http://shop.github.com">Shop</a></li>
      <li><a href="/blog">Blog</a></li>
      <li><a href="/about">About</a></li>

    </ul>

    <a href="/">
      <span class="mega-octicon octicon-mark-github"></span>
    </a>

    <ul class="site-footer-links">
      <li>&copy; 2013 <span title="0.05366s from github-fe127-cp1-prd.iad.github.net">GitHub</span>, Inc.</li>
        <li><a href="/site/terms">Terms</a></li>
        <li><a href="/site/privacy">Privacy</a></li>
        <li><a href="/security">Security</a></li>
        <li><a href="/contact">Contact</a></li>
    </ul>
  </div><!-- /.site-footer -->
</div><!-- /.container -->


    <div class="fullscreen-overlay js-fullscreen-overlay" id="fullscreen_overlay">
  <div class="fullscreen-container js-fullscreen-container">
    <div class="textarea-wrap">
      <textarea name="fullscreen-contents" id="fullscreen-contents" class="js-fullscreen-contents" placeholder="" data-suggester="fullscreen_suggester"></textarea>
          <div class="suggester-container">
              <div class="suggester fullscreen-suggester js-navigation-container" id="fullscreen_suggester"
                 data-url="/phalcon/zephir/suggestions/commit">
              </div>
          </div>
    </div>
  </div>
  <div class="fullscreen-sidebar">
    <a href="#" class="exit-fullscreen js-exit-fullscreen tooltipped leftwards" title="Exit Zen Mode">
      <span class="mega-octicon octicon-screen-normal"></span>
    </a>
    <a href="#" class="theme-switcher js-theme-switcher tooltipped leftwards"
      title="Switch themes">
      <span class="octicon octicon-color-mode"></span>
    </a>
  </div>
</div>



    <div id="ajax-error-message" class="flash flash-error">
      <span class="octicon octicon-alert"></span>
      <a href="#" class="octicon octicon-remove-close close ajax-error-dismiss"></a>
      Something went wrong with that request. Please try again.
    </div>

  </body>
</html>

