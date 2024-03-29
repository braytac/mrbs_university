<?php
namespace MRBS;

use MRBS\Form\Form;
use MRBS\Form\ElementInputDate;
use MRBS\Form\ElementInputSearch;
use MRBS\Form\ElementInputSubmit;

function print_head($simple=false)
{
  echo "<head>\n";
  
  echo "<meta charset=\"" . get_charset() . "\">\n";
  // Set IE=edge so that IE10 will display MRBS properly, even if compatibility mode is used
  // on the browser.  If we don't do this then MRBS will treat IE10 as an unsupported browser
  // when compatibility mode is turned on, potentially confusing users who may have forgotten
  // that they are using compatibility mode.   Unfortunately we can't set IE=edge in the header,
  // which is where we would normally do it, because then we won't be able to detect IE9 using
  // conditional comments.  So we have to do it in a <meta> tag, after the conditional comments
  // around the <html> tags.
  echo "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  
  if (!$simple)
  {
    // Add the CSRF token so that JavaScript can use it
    echo "<meta name=\"csrf_token\" content=\"" . htmlspecialchars(Form::getToken()) . "\">\n";
  }
  
  echo "<title>" . get_vocab("mrbs") . "</title>\n";
  
  require_once "style.inc";
  
  if (!$simple)
  {
    require_once "js.inc";
  }
?>
<!-- Magnific Popup core CSS file -->
<link rel="stylesheet" href="magnific-popup/dist/magnific-popup.css">

<!-- jQuery 1.7.2+ or Zepto.js 1.0+ -->
<!-- <script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>-->

<!-- Magnific Popup core JS file -->
<script src="magnific-popup/dist/jquery.magnific-popup.js"></script>
<script>
$(document).ready(function() {
  $('#rooms_table a.mgi').magnificPopup({type:'image'});
});
</script>
<?php  
  echo "</head>\n";
}


// Print the basic site information.   This function is used for all headers, including
// the simple header, and so mustn't require any database access.
function print_header_site_info()
{
  global $mrbs_company,
         $mrbs_company_url,
         $mrbs_company_logo,
         $mrbs_company_more_info;
  
  echo "<div class=\"company\">\n";
  
  echo "<div class=\"logo\">\n";
  if (!empty($mrbs_company_url))
  {
    echo "<a href=\"$mrbs_company_url\">\n";
  }
  if (empty($mrbs_company_logo))
  {
    echo "<span>$mrbs_company</span>\n";
  }
  else
  {
    // Suppress error messages in case the logo is a URL and allow_url_fopen
    // is not enabled in php.ini
    $logo_size = @getimagesize($mrbs_company_logo);
    echo "<a href=\"https://www.ing.unlp.edu.ar\" ><img src=\"$mrbs_company_logo\" style=\"vertical-align: middle;\" height=\"50\" alt=\"$mrbs_company\"></a>\n";
    echo "<a href=\"https://www.ing.unlp.edu.ar/sitio/aulas\">&nbsp;&nbsp;&nbsp;&nbsp;".get_vocab("mrbs")."</a>";
  }
  if (!empty($mrbs_company_url))
  {
    echo "</a>\n";
  }
  echo "</div>\n";
/*    
  if (!empty($mrbs_company_more_info))
  {
    echo "<div id=\"more_info\">$mrbs_company_more_info</div>\n";
  }


  echo "<div class=\"mrbs\">\n";
  echo "<a href=\"index.php\">" . get_vocab("mrbs") . "</a>\n";
  echo "</div>\n";
*/ 
  echo "</div>\n";
}


function print_goto_date($day, $month, $year, $area, $room)
{
  $user = getUserName();
  
  $form = new Form();
  
  $form_id = 'form_nav';
  
  $form->setAttributes(array('id'     => $form_id,
                             'method' => 'get',
                             'action' => 'day.php'));
  if (isset($area))
  {
    $form->addHiddenInput('area', $area);
  }
  
  if (isset($room))
  {
    $form->addHiddenInput('room', $room);
  }
  
  $input = new ElementInputDate();
  $input->setAttributes(array('name'        => 'page_date',
                              'value'       => format_iso_date($year, $month, $day),
                              'required'    => true,
                              'data-submit' => $form_id));
  
  $form->addElement($input);
  
  $submit = new ElementInputSubmit();
  $submit->setAttribute('value', get_vocab('goto'));
  $form->addElement($submit);
  
  $form->render();
  
  
  // Provide a link to the list of bookings awaiting approval
  // (if there are any enabled areas where we require bookings to be approved)
  $approval_somewhere = some_area('approval_enabled', TRUE);
  if ($approval_somewhere && (authGetUserLevel($user) >= 1))
  {
    $n_outstanding = get_entries_n_outstanding($user);
    echo "<div id=\"n_outstanding\"" .
         (($n_outstanding > 0) ? " class=\"outstanding\"" : '') .
         ">\n";
    echo "<a href=\"pending.php?day=$day&amp;month=$month&amp;year=$year&amp;area=$area" . 
         ((!empty($room)) ? "&amp;room=$room" : "") . 
         "\">$n_outstanding " . get_vocab("outstanding") . "</a>\n";
    echo "</div>\n";
  }
}


function print_help($query_string)
{
  echo "<a href=\"help.php?$query_string\">" . get_vocab("help") . "</a>\n";
}


function print_rooms($query_string)
{
  echo "<a href=\"admin.php?$query_string\">" . get_vocab("rooms") . "</a>\n";
}


function print_report($query_string)
{
  echo "<a href=\"report.php?$query_string\">" . get_vocab("report") . "</a>\n";
}


function print_search($day, $month, $year, $area, $room, $search_str=null)
{
  if (!isset($search_str))
  {
    $search_str = '';
  }
  
  echo "<label><a href=\"search.php?advanced=1\">" . get_vocab('search') . "</a></label>\n";
  
  $form = new Form();
  
  $form->setAttributes(array('id'     => 'header_search',
                             'method' => 'post',
                             'action' => 'search.php'))
       ->addHiddenInputs(array('day'   => $day,
                               'month' => $month,
                               'year'  => $year));
  if (!empty($area))
  {
    $form->addHiddenInput('area', $area);
  }
  if (!empty($room))
  {
    $form->addHiddenInput('room', $room);
  }
  
  $input = new ElementInputSearch();
  
  $input->setAttributes(array('name'     => 'search_str',
                              'value'    => $search_str,
                              'required' => true));
                              
  $form->addElement($input);
  
  $form->render();
}


function print_nav($day, $month, $year, $area, $room, $search_str=null, $simple=false)
{
  $query_string = "day=$day&amp;month=$month&amp;year=$year";
  if (!empty($area))
  {
    $query_string .= "&amp;area=$area";
  }
  if (!empty($room))
  {
    $query_string .= "&amp;room=$room";
  }
  
  echo "<nav>\n";
  echo "<ul>\n";
  
  echo "<li>\n";
  print_header_site_info();
  echo "</li>\n";
  
  if (!$simple)
  {
    echo "<li>\n";
    print_goto_date($day, $month, $year, $area, $room);
    echo "</li>\n";
/*    
    echo "<li>\n";
    print_help($query_string);
    echo "</li>\n";
*/    
    echo "<li>\n";
    print_rooms($query_string);
    echo "</li>\n";
if (isset($_SESSION['UserName'])) {     
    echo "<li>\n";
    print_report($query_string);
    echo "</li>\n";
}    
    echo "<li>\n";
    print_search($day, $month, $year, $area, $room, $search_str);
    echo "</li>\n";
    
    // For session protocols that define their own logon box...
    if (function_exists(__NAMESPACE__ . "\\print_logon_box"))
    {
      echo "<li id=\"logon_box\">\n";
      print_logon_box();
      echo "</li>\n";
    }
  }
  
  echo "</ul>\n";
  echo "</nav>\n";
}



// Print a message which will only be displayed (thanks to CSS) if the user is
// using an unsupported browser.
function print_unsupported_message()
{
  echo "<div class=\"unsupported_message\">\n";
  echo "<header class=\"banner simple\">\n";
  print_nav(null, null, null, null, null, null, $simple=true);
  echo "</header>\n";
  echo "<div class=\"contents\">\n";
  echo "<p>" . get_vocab('browser_not_supported', get_vocab('mrbs_abbr')) . "</p>\n";
  echo "</div>\n";
  echo "</div>\n";
}


// Print the page header
// If $simple is true, then just print a simple header that doesn't require any database
// access or JavaScript (useful for fatal errors and database upgrades).
function print_theme_header($day, $month, $year, $area, $room, $simple=false, $search_str=null)
{
  // Get the form token now, before any headers are sent, in case we are using the 'cookie'
  // session scheme.  Otherwise we won't be able to store the Form token.
  Form::getToken();
  
  $headers = array("Content-Type: text/html; charset=" . get_charset());
  http_headers($headers);
  
  echo DOCTYPE . "\n";;

  // We produce two <html> tags: one for versions of IE that we don't support and one for all
  // other browsers.  This enables us to use CSS to hide and show the appropriate text.
  echo "<!--[if lte IE 9]>\n";
  echo "<html lang=\"" . htmlspecialchars(get_lang()) . "\" class=\"unsupported_browser\">\n";
  echo "<![endif]-->\n";
  echo "<!--[if (!IE)|(gt IE 9)]><!-->\n";
  echo "<html lang=\"" . htmlspecialchars(get_lang()) . "\">\n";
  echo "<!--<![endif]-->\n";
  
  print_head($simple);

  // Put the filename in as a class to aid styling.   
  // (Use a class rather than id to avoid specificity problems)
  echo "<body class=\"non_js ".htmlspecialchars(this_page('.php'))."\">\n";
   
  if (!$simple)
  {
    // Add a class of "js" so that we know if we're using JavaScript or not
    // and remove the non_js class (it's sometimes useful to know that we're
    // not running JavaScript)
    ?>
    <script type="text/javascript">
      //<![CDATA[
      $('body').addClass('js').removeClass('non_js');
      //]]>
    </script> 
    <?php
  }
  
  $class = 'banner';
  if ($simple)
  {
    $class .= ' simple';
  }
  
  print_unsupported_message();
  
  echo "<header class=\"$class\">\n";
  print_nav($day, $month, $year, $area, $room, $search_str, $simple);
  echo "</header>\n";

  // This <div> should really be moved out of here so that we can always see
  // the matching closing </div>
  echo "<div class=\"contents\">\n";


} // end of print_theme_header()

