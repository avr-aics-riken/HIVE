import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティ、ノードタイトル.
 */
export default class ItemTitle extends React.Component {
	constructor(props) {
		super(props);
	}

	styles() {
		return {
			title : {
                backgroundColor : "rgb(33, 187, 151)",
                borderRadius: "2px",
                color : "#011",
                lineHeight: "24px",
                margin: "2px",
                padding: "0px 2px",
				paddingLeft : "4px",
                height: "24px",
			}
		};
	}

	render () {
		const styles = this.styles.bind(this)();
        return (
            <div style={styles.title}>
				<span style={{marginLeft : "12px"}} title={this.props.initialParam.hasOwnProperty('value') ? this.props.initialParam.value : "(Object)"}>
                {this.props.initialParam.hasOwnProperty('value') ? this.props.initialParam.value : "(Object)"}
				</span>
				{this.props.exportButton}
            </div>
        );
	}
}
