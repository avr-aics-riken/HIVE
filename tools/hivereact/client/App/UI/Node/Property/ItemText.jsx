import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(Text)ビュー.
 */
export default class ItemText extends React.Component {
	constructor(props) {
		super(props);
	}

	styles() {
        let border = ()=>{
            if(this.props.top && this.props.bottom){
                return {
                    borderRadius: "2px 2px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.top){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "2px 2px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.bottom){
                return {
                    border: "none",
                    borderRadius: "0px 0px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else{
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }
        };
        return {
            view : border.bind(this)(),
            key : {
                backgroundColor: "rgb(84,84,84)",
                color : "white",
                fontSize: "smaller",
                letterSpacing: "normal",
                textAlign: this.props.initialParam.name.match(/^\[\d\]$/) ? "right" : "left",
                padding: "1px",
				paddingLeft : "5px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                overflow: "hidden",
                textShadow: "0px 0px 3px black"
            },
            value : {
                border: "0px",
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "3px",
				marginTop: "1px",
				marginBottom: "1px",
                verticalAlign: "middle",
                width : "162px",
                height: "20px",
                verticalAlign: "middle",
                display: "inline-block",
            }
        };
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.props.initialParam.name}
					</div>
					<div style={styles.value}>
						{this.props.initialParam.hasOwnProperty('value') ? this.props.initialParam.value : "(Object)"}
					</div>
				</div>);
	}
}
