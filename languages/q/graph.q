m:`a`d`c`b`e`f`g`h`j`k!(enlist `d;enlist `c;`b`e`f;enlist `a;`symbol$();`g`h;`symbol$();enlist `j;enlist `k;enlist `f);

visited:`symbol$();

deadend1:{[g;x]
    if [x in visited; :`symbol$()];

    visited::distinct visited,x;
    if [0=count g[x]; :enlist x];
    
    distinct raze deadend1[g;] each g[x]
    };    
