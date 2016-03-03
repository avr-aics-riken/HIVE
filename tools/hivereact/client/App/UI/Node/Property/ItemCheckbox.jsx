import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(Checkbox)ビュー.
 */
export default class ItemCheckbox extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			checked : this.props.initialParam.value
		};
	}

    styles() {
        let border = ()=>{
            if(this.props.top){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "3px 3px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.bottom){
                return {
                    border: "none",
                    borderRadius: "0px 0px 3px 3px",
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
                backgroundColor: "#666",
                color : "white",
                fontSize: "smaller",
                letterSpacing: "normal",
                padding: "1px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                textShadow: "0px 0px 3px black"
            },
            value : {
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "1px",
                padding: "1px",
                width : "160px",
                verticalAlign: "middle",
                display: "inline-block",
            }
        };
    }

	onChange(ev) {
		this.props.changeCheckboxFunc(this.props.initialParam.name, ev.target.checked);
		this.setState({
			checked : ev.target.checked
		});
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.props.initialParam.name}
					</div>
					<input type="checkbox"
					 	style={styles.value}
						checked={this.props.initialParam.value}
						onChange={this.onChange.bind(this)}
					>
					</input>
				</div>);
	}
}
