module.exports = require('./Splitter.jsx');

// = splitter =================================================================
//
// Splitter コンポーネントにブロック要素をふたつ食わせる（ふたつ固定）
//
// ============================================================================
//
// split          : string        : horizontarl : スプリッタを水平に置く
//                :               : vertical    : スプリッタを垂直に置く
// minSize        : int or string : 左（もしくは上）のひとつめの Pane の最小幅（もしくは高さ）
// defaultSize    : int or string : ひとつめの Pane のデフォルトの大きさ
// secondPaneSize : int or string : 右（もしくは下）のふたつめの Pane の大きさ
// lockSecondPane : bool          : ふたつめの Pane を幅固定にするかどうかのフラグ
//                                : 若干わかりにくいのだが、たとえば下に固定して
//                                : ウィンドウサイズを変えても常に Bottom 0px な
//                                : 位置に出てほしいときとかにつかう
// dontmove       : bool          : スプリッタを可動式とするかどうかのフラグ
// overflow1      : string        : pane1 の overflow を設定する（既定値は auto）
// overflow2      : string        : pane2 の overflow を設定する（既定値は auto）
//
// ============================================================================


