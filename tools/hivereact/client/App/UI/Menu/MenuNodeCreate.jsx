import React from "react"
import ReactDOM from "react-dom"
import Autosuggest from 'react-autosuggest';
import Core from '../../Core'

export default class MenuNodeCreate extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes : null,
            value: '',
            suggestions: this.getSuggestions('')
        };

        this.styles = this.styles.bind(this);

        // auto suggest
        this.onChange = this.onChange.bind(this);
        this.onSuggestionsUpdateRequested = this.onSuggestionsUpdateRequested.bind(this);
        this.getSuggestions = this.getSuggestions.bind(this);
        this.getSuggestionValue = this.getSuggestionValue.bind(this);
        this.renderSuggestion = this.renderSuggestion.bind(this);
    }

    // インプットボックスの change と自身のステートの更新はここ
    onChange(eve, {newValue}){
        this.setState({value: newValue});
    }

    // ここにある nodeNames にセットされた名前がサジェストされる
    getSuggestions(value) {
        var nodeNameList = this.props.nodeSystem.GetNodeNameList();
        var nodeNames = [];
        if(nodeNameList){
            for(let i in nodeNameList){
                nodeNames.push({name: nodeNameList[i]});
            }
        }
        if(nodeNames.length === 0){return [];}
        const inputValue = value.trim().toLowerCase();
        const inputLength = inputValue.length;
        return inputLength === 0 ? [] : nodeNames.filter(nodename =>
             nodename.name.toLowerCase().slice(0, inputLength) === inputValue
        );
    }
    onSuggestionsUpdateRequested({value}){this.setState({suggestions: this.getSuggestions(value)});}
    getSuggestionValue(suggestion){return suggestion.name;}
    renderSuggestion(suggestion){return (<span>{suggestion.name}</span>);}

    styles() {
        return {
            block: {
                margin: "0px",
                padding: "0px",
                width: "100%"
            },
            suggestTheme: {
                // container:                   'react-autosuggest__container',
                // containerOpen:               'react-autosuggest__container--open',
                // input:                       'react-autosuggest__input',
                // suggestionsContainer:        'react-autosuggest__suggestions-container',
                // suggestion:                  'react-autosuggest__suggestion',
                // suggestionFocused:           'react-autosuggest__suggestion--focused',
                // sectionContainer:            'react-autosuggest__section-container',
                // sectionTitle:                'react-autosuggest__section-title',
                // sectionSuggestionsContainer: 'react-autosuggest__section-suggestions-container'
                input: {
                    backgroundColor: "black",
                    border: "none",
                    color: "whitesmoke",
                    width: "100%",
                    margin: "0px",
                    padding: "8px 0px"
                },
                suggestionsContainer: {
                    listStyle: "none",
                    margin: "0px",
                    padding: "0px",
                    width: "100%",
                    position: "absolute"
                },
                suggestion: {
                    backgroundColor: "rgba(0, 0, 0, 0.7)",
                    borderLeft: "1px solid gray",
                    borderRight: "1px solid gray",
                    borderBottom: "1px solid gray",
                    fontSize: "large",
                    margin: "0px",
                    padding: "5px",
                    overflow: "hidden"
                },
                suggestionFocused: {
                    backgroundColor: "rgba(64, 64, 64, 0.5)"
                }
            }
        }
    }

    // 以下 placeholder に与えた文字列がインプットボックスの
    // なかに初期値として見えている文字列
    render(){
        const style = this.styles();
        const {value, suggestions} = this.state;
        const inputProps = {
            placeholder: 'Type a Node name.',
            value,
            onChange: this.onChange
        };
        return (
            <div style={style.block}>
                <Autosuggest theme={style.suggestTheme}
                    suggestions={suggestions}
                    onSuggestionsUpdateRequested={this.onSuggestionsUpdateRequested}
                    getSuggestionValue={this.getSuggestionValue}
                    renderSuggestion={this.renderSuggestion}
                    inputProps={inputProps} />
            </div>
        );
    }
}


